// Copyright Dry Horns 


#include "LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/Abilities/LKGameplayAbility.h"
#include "Aura/LKSLogChannels.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Aura/Interaction/LKPlayerInterface.h"
#include "Aura/LKGameplayTags.h"
#include "Aura/AbilitySystem/LKAbilitySystemLibrary.h"
#include "Aura/AbilitySystem/Data/LKAbilityInfo.h"

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
			AbilitySpec.DynamicAbilityTags.AddTag(FLKGameplayTags::Get().Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
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

FGameplayTag ULKAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

FGameplayAbilitySpec* ULKAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return nullptr;
}

void ULKAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<ULKPlayerInterface>())
	{
		if (ILKPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ServerUpgradeAttribute(AttributeTag);
		}
	}
}

void ULKAbilitySystemComponent::ServerUpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	if (GetAvatarActor()->Implements<ULKPlayerInterface>())
	{
		ILKPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void ULKAbilitySystemComponent::UpdateAbilityStatuses(int32 Level)
{
	ULKAbilityInfo* AbilityInfo = ULKAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FLKSAbilityInfo& Info : AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid())
		{
			continue;
		}
		if (Level < Info.LevelRequirement) 
		{
			continue;
		}

		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.Ability, 1);
			AbilitySpec.DynamicAbilityTags.AddTag(FLKGameplayTags::Get().Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ClientUpdateAbilityStatus(Info.AbilityTag, FLKGameplayTags::Get().Abilities_Status_Eligible);
		}
	}
}

void ULKAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void ULKAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag);
}
