// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * LK gameplay tags 
 * Singleton containg native Gameplay Tags
 */
struct FLKGameplayTags
{
	public:
		static const FLKGameplayTags& Get() { return GameplayTags; }
		static void InitializeNativeGameplayTags();

		FGameplayTag Attributes_Primary_Strength;
		FGameplayTag Attributes_Primary_Intelligence;
		FGameplayTag Attributes_Primary_Resilience;
		FGameplayTag Attributes_Primary_Vigor;
		FGameplayTag Attributes_Primary_Stamina;

		FGameplayTag Attributes_Secondary_Armor;
		FGameplayTag Attributes_Secondary_ArmorPenetration;
		FGameplayTag Attributes_Secondary_BlockChance;
		FGameplayTag Attributes_Secondary_CriticalHitChance;
		FGameplayTag Attributes_Secondary_CriticalHitDamage;
		FGameplayTag Attributes_Secondary_CriticalHitResistance;
		FGameplayTag Attributes_Secondary_HealthRegeneration;
		FGameplayTag Attributes_Secondary_ManaRegeneration;
		FGameplayTag Attributes_Secondary_MaxHealth;
		FGameplayTag Attributes_Secondary_MaxMana;

		FGameplayTag InputTag_LMB;
		FGameplayTag InputTag_RMB;
		FGameplayTag InputTag_1;
		FGameplayTag InputTag_2;
		FGameplayTag InputTag_3;
		FGameplayTag InputTag_4;

		FGameplayTag Damage;
		FGameplayTag Effects_HitReact;

		TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
		
		FGameplayTag Damage_Fire;
		FGameplayTag Damage_Lightning;
		FGameplayTag Damage_Arcane;
		FGameplayTag Damage_Physical;

		FGameplayTag Attributes_Resistance_Fire;
		FGameplayTag Attributes_Resistance_Lightning;
		FGameplayTag Attributes_Resistance_Arcane;
		FGameplayTag Attributes_Resistance_Physical;
	protected:

	private:
		static FLKGameplayTags GameplayTags;
};
