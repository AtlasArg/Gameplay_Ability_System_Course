// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "LkCharacterBase.h"
#include "Aura/Interaction/LKPlayerInterface.h"
#include "LkPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ALkPlayerCharacter : public ALkCharacterBase, public ILKPlayerInterface
{
	GENERATED_BODY()

public:
	ALkPlayerCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/** Players Interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	/** end Player Interface */

	virtual int32 GetPlayerLevel() override;

private:
	UPROPERTY(EditAnywhere, Category = "Character settings")
	FRotator RotationRate = FRotator(0.f, 400.f, 0.f);

	virtual void InitAbilityActorInfo() override;
};
