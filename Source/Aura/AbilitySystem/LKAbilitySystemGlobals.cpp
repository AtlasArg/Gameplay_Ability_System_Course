// Copyright Dry Horns 


#include "LKAbilitySystemGlobals.h"
#include "Aura/LKAbilityTypes.h"

FGameplayEffectContext* ULKAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FLKGameplayEffectContext();
}
