// Copyright Dry Horns 


#include "LKSpellMenuWidgetController.h"
#include "Aura/AbilitySystem/Data/LKAbilityInfo.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/Player/LKPlayerState.h"

void ULKSpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
    SpellPointsChanged.Broadcast(GetLKPS()->GetSpellPoints());
}

void ULKSpellMenuWidgetController::BindCallbacksToDependencies()
{
    GetLKASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
        {
            if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
            {
                SelectedAbility.Status = StatusTag;
                bool bEnableSpendPoints = false;
                bool bEnableEquip = false;
                ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
                FString Description;
                FString NextLevelDescription;
                GetLKASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
                SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
            }

            if (AbilityInfo)
            {
                FLKSAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
                Info.StatusTag = StatusTag;
                AbilityInfoDelegate.Broadcast(Info);
            }
        });

    GetLKASC()->AbilityEquipped.AddUObject(this, &ThisClass::OnAbilityEquipped);
    GetLKPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
        {
            SpellPointsChanged.Broadcast(SpellPoints);
            CurrentSpellPoints = SpellPoints;

            bool bEnableSpendPoints = false;
            bool bEnableEquip = false;
            ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
            FString Description;
            FString NextLevelDescription;
            GetLKASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
            SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
        });
}

void ULKSpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
    if (bWaitingForEquipSelection)
    {
        const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
        StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
        bWaitingForEquipSelection = false;
    }

    const FLKGameplayTags GameplayTags = FLKGameplayTags::Get();
    const int32 SpellPoints = GetLKPS()->GetSpellPoints();
    FGameplayTag AbilityStatus;

    const bool bTagValid = AbilityTag.IsValid();
    const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
    const FGameplayAbilitySpec* AbilitySpec = GetLKASC()->GetSpecFromAbilityTag(AbilityTag);
    const bool bSpecValid = AbilitySpec != nullptr;
    if (!bTagValid || bTagNone || !bSpecValid)
    {
        AbilityStatus = GameplayTags.Abilities_Status_Locked;
    }
    else
    {
        AbilityStatus = GetLKASC()->GetStatusFromSpec(*AbilitySpec);
    }

    SelectedAbility.Ability = AbilityTag;
    SelectedAbility.Status = AbilityStatus;
    bool bEnableSpendPoints = false;
    bool bEnableEquip = false;
    ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpendPoints, bEnableEquip);
    FString Description;
    FString NextLevelDescription;
    GetLKASC()->GetDescriptionsByAbilityTag(AbilityTag, Description, NextLevelDescription);
    SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip, Description, NextLevelDescription);
}

void ULKSpellMenuWidgetController::SpendPointButtonPressed()
{
    if (GetLKASC())
    {
        GetLKASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
    }
}

void ULKSpellMenuWidgetController::GlobeDeselect()
{
    if (bWaitingForEquipSelection)
    {
        const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
        StopWaitingForEquipDelegate.Broadcast(SelectedAbilityType);
        bWaitingForEquipSelection = false;
    }

    SelectedAbility.Ability = FLKGameplayTags::Get().Abilities_None;
    SelectedAbility.Status = FLKGameplayTags::Get().Abilities_Status_Locked;

    SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void ULKSpellMenuWidgetController::EquipButtonPressed()
{
    const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;

    WaitForEquipDelegate.Broadcast(AbilityType);
    bWaitingForEquipSelection = true;

    const FGameplayTag SelectedStatus = GetLKASC()->GetStatusFromAbilityTag(SelectedAbility.Ability);
    if (SelectedStatus.MatchesTagExact(FLKGameplayTags::Get().Abilities_Status_Equipped))
    {
        SelectedSlot = GetLKASC()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
    }
}

void ULKSpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
    if (!bWaitingForEquipSelection) return;
    // Check selected ability against the slot's ability type.
    // (don't equip an offensive spell in a passive slot and vice versa)
    const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
    if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;

    GetLKASC()->ServerEquipAbility(SelectedAbility.Ability, SlotTag);
}

void ULKSpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
    bWaitingForEquipSelection = false;

    const FLKGameplayTags& GameplayTags = FLKGameplayTags::Get();

    FLKSAbilityInfo LastSlotInfo;
    LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
    LastSlotInfo.InputTag = PreviousSlot;
    LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
    // Broadcast empty info if PreviousSlot is a valid slot. Only if equipping an already-equipped spell
    AbilityInfoDelegate.Broadcast(LastSlotInfo);

    FLKSAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
    Info.StatusTag = Status;
    Info.InputTag = Slot;
    AbilityInfoDelegate.Broadcast(Info);

    StopWaitingForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType);
    SpellGlobeReassignedDelegate.Broadcast(AbilityTag);
    GlobeDeselect();
}

void ULKSpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
    const FLKGameplayTags GameplayTags = FLKGameplayTags::Get();

    bShouldEnableSpellPointsButton = false;
    bShouldEnableEquipButton = false;
    if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
    {
        bShouldEnableEquipButton = true;
        if (SpellPoints > 0)
        {
            bShouldEnableSpellPointsButton = true;
        }
    }
    else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
    {
        if (SpellPoints > 0)
        {
            bShouldEnableSpellPointsButton = true;
        }
    }
    else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
    {
        bShouldEnableEquipButton = true;
        if (SpellPoints > 0)
        {
            bShouldEnableSpellPointsButton = true;
        }
    }
}
