// Copyright Dry Horns 


#include "LKAbilitySystemComponent.h"
#include "Aura//AbilitySystem/Abilities/LKGameplayAbility.h"
#include "Aura/LKSLogChannels.h"

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

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void ULKAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
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

void ULKAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag ULKAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag ULKAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

void ULKAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}
}
