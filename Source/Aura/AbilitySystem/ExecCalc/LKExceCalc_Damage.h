// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "LKExceCalc_Damage.generated.h"

 
UCLASS()
class AURA_API ULKExceCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	ULKExceCalc_Damage();

	void DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		const FGameplayEffectSpec& Spec,
		FAggregatorEvaluateParameters EvaluationParameters,
		const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs) const;

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

};
