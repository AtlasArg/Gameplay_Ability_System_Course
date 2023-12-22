// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "LKMMCMaxMana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULKMMCMaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	ULKMMCMaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:

	FGameplayEffectAttributeCaptureDefinition IntDefinition;

};
