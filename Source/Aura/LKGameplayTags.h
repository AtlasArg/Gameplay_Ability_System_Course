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

		FGameplayTag Attributes_Meta_IncomingXP;

		FGameplayTag Abilities_HitReact;

		FGameplayTag Player_Block_InputPressed;
		FGameplayTag Player_Block_InputHeld;
		FGameplayTag Player_Block_InputReleased;
		FGameplayTag Player_Block_CursorTrace;

		FGameplayTag Abilities_Status_Locked;
		FGameplayTag Abilities_Status_Eligible;
		FGameplayTag Abilities_Status_Unlocked;
		FGameplayTag Abilities_Status_Equipped;

		FGameplayTag Abilities_Type_Offensive;
		FGameplayTag Abilities_Type_Passive;
		FGameplayTag Abilities_Type_None;

		FGameplayTag InputTag_LMB;
		FGameplayTag InputTag_RMB;
		FGameplayTag InputTag_1;
		FGameplayTag InputTag_2;
		FGameplayTag InputTag_3;
		FGameplayTag InputTag_4;
		FGameplayTag InputTag_Passive_1;
		FGameplayTag InputTag_Passive_2;

		FGameplayTag Damage;
		FGameplayTag Effects_HitReact;

		TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
		TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
		
		FGameplayTag Damage_Fire;
		FGameplayTag Damage_Lightning;
		FGameplayTag Damage_Arcane;
		FGameplayTag Damage_Physical;

		FGameplayTag Attributes_Resistance_Fire;
		FGameplayTag Attributes_Resistance_Lightning;
		FGameplayTag Attributes_Resistance_Arcane;
		FGameplayTag Attributes_Resistance_Physical;

		FGameplayTag Debuff_Burn;
		FGameplayTag Debuff_Stun;
		FGameplayTag Debuff_Arcane;
		FGameplayTag Debuff_Physical;

		FGameplayTag Debuff_Chance;
		FGameplayTag Debuff_Damage;
		FGameplayTag Debuff_Duration;
		FGameplayTag Debuff_Frequency;

		FGameplayTag Abilities_Attack;
		FGameplayTag Abilities_None;

		FGameplayTag Abilities_Fire_FireBolt;
		FGameplayTag Abilities_Lightning_Electrocute;
		FGameplayTag Cooldown_Fire_FireBolt;

		FGameplayTag Montage_Attack_Weapon;
		FGameplayTag Montage_Attack_RightHand;
		FGameplayTag Montage_Attack_LeftHand;

	protected:

	private:
		static FLKGameplayTags GameplayTags;
};
