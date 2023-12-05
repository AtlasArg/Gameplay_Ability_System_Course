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

protected:
	virtual void BeginPlay() override;
};
