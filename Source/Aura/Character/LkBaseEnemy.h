// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "LkCharacterBase.h"
#include "Aura/Interaction/LKEnemyInterface.h"
#include "LkBaseEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ALkBaseEnemy : public ALkCharacterBase, public ILKEnemyInterface
{
	GENERATED_BODY()
	
public:
	ALkBaseEnemy();

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	virtual int32 GetPlayerLevel() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

};
