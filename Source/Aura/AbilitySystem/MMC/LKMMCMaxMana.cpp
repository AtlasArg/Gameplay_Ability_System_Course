// Copyright Dry Horns 


#include "LKMMCMaxMana.h"

#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/Interaction/LKCombatInterface.h"

ULKMMCMaxMana::ULKMMCMaxMana()
{
	IntDefinition.AttributeToCapture = ULKAttributeSet::GetIntelligenceAttribute();
	IntDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntDefinition);
}

float ULKMMCMaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Int = 0.f;
	GetCapturedAttributeMagnitude(IntDefinition, Spec, EvaluationParameters, Int);
	Int = FMath::Max<float>(Int, 0.f);

	int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<ULKCombatInterface>())
	{
		PlayerLevel = ILKCombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}

	return 50.f + 2.5f * Int + 15.f * PlayerLevel;
}