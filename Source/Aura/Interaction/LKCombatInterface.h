// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LKCombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class ULKCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class UAnimMontage;

class AURA_API ILKCombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual int32 GetPlayerLevel();

	virtual FVector GetCombatSocketLocation();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateFixingTarget(const FVector& Target);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;
};
