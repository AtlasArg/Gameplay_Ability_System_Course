// Copyright Dry Horns 


#include "LKAbilitySystemComponent.h"
#include "Aura//AbilitySystem/Abilities/LKGameplayAbility.h"
#include "Aura/LKGameplayTags.h"

void ULKAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ULKAbilitySystemComponent::ClientEffectApplied);
}

void ULKAbilitySystemComponent::AddCharacterAbilites(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		const ULKGameplayAbility* LKAbility = Cast<ULKGameplayAbility>(AbilitySpec.Ability);
		if (LKAbility)
		{
			AbilitySpec.DynamicAbilityTags.AddTag(LKAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

void ULKAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	OnEffectAssetTags.Broadcast(TagContainer);
}

void ULKAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		TArray<FGameplayAbilitySpec> Abilities = GetActivatableAbilities();
		for (auto& AbilitySpec : Abilities)
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				AbilitySpecInputPressed(AbilitySpec);
				if (!AbilitySpec.IsActive())
				{
					TryActivateAbility(AbilitySpec.Handle);
				}
			}
		}
	}
}

void ULKAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		TArray<FGameplayAbilitySpec> Abilities = GetActivatableAbilities();
		for (auto& AbilitySpec : Abilities)
		{
			if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
			{
				AbilitySpecInputReleased(AbilitySpec);
			}
		}
	}
}