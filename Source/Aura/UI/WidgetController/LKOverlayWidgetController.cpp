// Copyright Dry Horns 


#include "LKOverlayWidgetController.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/LKSLogChannels.h"
#include "Aura/AbilitySystem/Data/LKAbilityInfo.h"
#include "Aura/AbilitySystem/Data/LevelUpInfo.h"
#include "Aura/Player/LKPlayerState.h"
#include "Aura/LKGameplayTags.h"

void ULKOverlayWidgetController::BroadcastInitialValues()
{

	OnHealthChanged.Broadcast(GetLKAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetLKAS()->GetMaxHealth());

	OnManaChanged.Broadcast(GetLKAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetLKAS()->GetMaxMana());
}

void ULKOverlayWidgetController::BindCallbacksToDependencies()
{
	GetLKPS()->OnXPChangedDelegate.AddUObject(this, &ULKOverlayWidgetController::OnXPChanged);
	GetLKPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel)
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetLKAS()->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetLKAS()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetLKAS()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetLKAS()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (GetLKASC())
	{
		GetLKASC()->AbilityEquipped.AddUObject(this, &ThisClass::OnAbilityEquipped);
		if (GetLKASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetLKASC()->AbilitiesGivenDelegate.AddUObject(this, &ULKOverlayWidgetController::BroadcastAbilityInfo);
		}

		Cast<ULKAbilitySystemComponent>(GetLKASC())->OnEffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					// For example, say that Tag = Message.HealthPotion
					// "Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRow.Broadcast(*Row);
					}
				}
			});
	}
}

void ULKOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ALKPlayerState* AuraPlayerState = CastChecked<ALKPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unabled to find LevelUpInfo. Please fill out AuraPlayerState Blueprint"));

	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void ULKOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
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
}
