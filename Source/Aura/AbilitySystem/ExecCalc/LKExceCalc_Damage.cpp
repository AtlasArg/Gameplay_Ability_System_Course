// Copyright Dry Horns 


#include "LKExceCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/LKGameplayTags.h"
#include "Aura/AbilitySystem/LKAbilitySystemLibrary.h"
#include "Aura/Interaction/LKCombatInterface.h"


struct LKDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	LKDamageStatics() 
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULKAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULKAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULKAttributeSet, BlockChance, Target, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(ULKAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULKAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(ULKAttributeSet, CriticalHitDamage, Source, false);
	}
};

static const LKDamageStatics& DamageStatics()
{
	static LKDamageStatics DStatics;

	return DStatics;
}

ULKExceCalc_Damage::ULKExceCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void ULKExceCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ILKCombatInterface* SourceCombatInterface = Cast<ILKCombatInterface>(SourceAvatar);
	ILKCombatInterface* TargetCombatInterface = Cast<ILKCombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// Get damage set by caller magnitude
	float Damage = Spec.GetSetByCallerMagnitude(FLKGameplayTags::Get().Damage);

	// capture block chance on target and check it. If block, halve the damage
	float TargetBlockChance = 0.f; 
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);

	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	Damage = bBlocked ? Damage / 2.f : Damage;

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(0.f, SourceArmorPenetration);

	const UCharacterClassInfo* CharacterClassInfo = ULKAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoef->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoef = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoef->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoef = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoef) / 100.f;
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoef) / 100.f;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoef->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoef = CriticalHitResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	// Cirical hit resistance reduces critical hit chance by a certain percentage
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoef;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	// double damage plus a bonus if critical hit.
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(ULKAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
