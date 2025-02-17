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

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const;

};
