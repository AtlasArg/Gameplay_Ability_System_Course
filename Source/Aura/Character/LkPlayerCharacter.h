// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "LkCharacterBase.h"
#include "LkPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ALkPlayerCharacter : public ALkCharacterBase
{
	GENERATED_BODY()

public:
	ALkPlayerCharacter();

private:
	UPROPERTY(EditAnywhere, Category = "Character settings")
	FRotator RotationRate = FRotator(0.f, 400.f, 0.f);
};
