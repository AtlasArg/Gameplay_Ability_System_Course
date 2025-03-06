// Copyright Dry Horns 


#include "LKOverlayWidgetController.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/LKSLogChannels.h"
#include "Aura/AbilitySystem/Data/LKAbilityInfo.h"

void ULKOverlayWidgetController::BroadcastInitialValues()
{
	const ULKAttributeSet* LKAttributeSet = CastChecked<ULKAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(LKAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(LKAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(LKAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(LKAttributeSet->GetMaxMana());
}

void ULKOverlayWidgetController::BindCallbacksToDependencies()
{
	const ULKAttributeSet* LKAttributeSet = CastChecked<ULKAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);

	if (ULKAbilitySystemComponent* ASC = Cast<ULKAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (ASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(ASC);
		}
		else
		{
			ASC->AbilitiesGivenDelegate.AddUObject(this, &ULKOverlayWidgetController::OnInitializeStartupAbilities);
		}

		Cast<ULKAbilitySystemComponent>(AbilitySystemComponent)->OnEffectAssetTags.AddLambda(
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

void ULKOverlayWidgetController::OnInitializeStartupAbilities(ULKAbilitySystemComponent* LKAbilitySystemComponent)
{
	//TODO Get information about all given abilities, look up their Ability Info, and broadcast it to widgets.
	if (!LKAbilitySystemComponent->bStartupAbilitiesGiven)
	{
		return;
	}

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, LKAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
		{
			//TODO need a way to figure out the ability tag for a given ability spec.
			FLKSAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(LKAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = LKAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});
	LKAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}
