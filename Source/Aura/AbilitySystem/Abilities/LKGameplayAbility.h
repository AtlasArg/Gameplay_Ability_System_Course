// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "LKGameplayAbility.generated.h"


UCLASS()
class AURA_API ULKGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
};
