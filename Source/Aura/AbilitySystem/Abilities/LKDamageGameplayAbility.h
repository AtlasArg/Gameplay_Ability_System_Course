// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "LKGameplayAbility.h"
#include "Aura/Interaction/LKCombatInterface.h"
#include "Aura/LKAbilityTypes.h"
#include "LKDamageGameplayAbility.generated.h"

UCLASS()
class AURA_API ULKDamageGameplayAbility : public ULKGameplayAbility
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 60.f;

	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;
};
