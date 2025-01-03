// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "LKPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class ILKEnemyInterface;
class ULKInputConfig;
class ULKAbilitySystemComponent;
class USplineComponent;
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
	void AutoRun();

	ILKEnemyInterface* LastActor;
	ILKEnemyInterface* ThisActor;
	FHitResult CursorHit;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<ULKInputConfig> InputConfig;

	/*UPROPERTY(EditDefaultsOnly, Category = "Input")*/
	TObjectPtr<ULKAbilitySystemComponent> AbilitySystemComponent;

	ULKAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreashold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAccpetanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr <USplineComponent> Spline;
};
