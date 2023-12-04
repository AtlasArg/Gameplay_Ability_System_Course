// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LkCharacterBase.generated.h"

UCLASS(Abstract)
class AURA_API ALkCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALkCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
};
