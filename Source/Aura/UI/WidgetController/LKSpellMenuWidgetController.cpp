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
                SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
            }

            if (AbilityInfo)
            {
                FLKSAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
                Info.StatusTag = StatusTag;
                AbilityInfoDelegate.Broadcast(Info);
            }
        });

    GetLKPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
        {
            SpellPointsChanged.Broadcast(SpellPoints);
            CurrentSpellPoints = SpellPoints;

            bool bEnableSpendPoints = false;
            bool bEnableEquip = false;
            ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpendPoints, bEnableEquip);
            SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
        });
}

void ULKSpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
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
    SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints, bEnableEquip);
}

void ULKSpellMenuWidgetController::SpendPointButtonPressed()
{
    if (GetLKASC())
    {
        GetLKASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
    }
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
