// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "LKGameplayAbility.h"
#include "LKDamageGameplayAbility.generated.h"

UCLASS()
class AURA_API ULKDamageGameplayAbility : public ULKGameplayAbility
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
