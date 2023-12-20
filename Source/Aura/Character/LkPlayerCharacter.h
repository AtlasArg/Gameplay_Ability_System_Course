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
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	UPROPERTY(EditAnywhere, Category = "Character settings")
	FRotator RotationRate = FRotator(0.f, 400.f, 0.f);

	virtual void InitAbilityActorInfo() override;
};
