// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LKSAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class AURA_API ALKSAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALKSAIController();

protected:

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
