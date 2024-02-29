// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LKAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);

UCLASS()
class AURA_API ULKAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AbilityActorInfoSet();

	FEffectAssetTags OnEffectAssetTags;

	UFUNCTION()
	void AddCharacterAbilites(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);

	UFUNCTION()
	void AbilityInputTagHeld(const FGameplayTag& InputTag);

	UFUNCTION()
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:

	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
