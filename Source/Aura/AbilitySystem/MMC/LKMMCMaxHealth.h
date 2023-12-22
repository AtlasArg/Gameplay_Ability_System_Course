// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "LKMMCMaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULKMMCMaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	ULKMMCMaxHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition VigorDefinition;
};
