// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "Aura/AbilitySystem/Abilities/LKProjectileSpell.h"
#include "LKFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULKFireBolt : public ULKProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
