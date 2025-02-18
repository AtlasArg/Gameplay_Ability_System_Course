// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "LKAbilitySystemGlobals.generated.h"

UCLASS()
class AURA_API ULKAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
