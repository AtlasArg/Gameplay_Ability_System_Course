// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LKPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ILKEnemyInterface;
struct FInputActionValue;

UCLASS()
class AURA_API ALKPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ALKPlayerController();
	virtual void PlayerTick(float DeltaTime) override;


protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere, Category="Input")
	TObjectPtr<UInputMappingContext> PlayerContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	ILKEnemyInterface* LastActor;
	ILKEnemyInterface* ThisActor;
};
