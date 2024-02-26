// Copyright Dry Horns 


#include "LKPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Aura/Input/LKInputComponent.h"
#include "Aura/Input/LKInputConfig.h"
#include "Aura/Interaction/LKEnemyInterface.h"

ALKPlayerController::ALKPlayerController()
{
	// TODO: this may not be needed for LK. cause its not multiplayer.
	bReplicates = true;
}

void ALKPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void ALKPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	check(PlayerContext);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem != nullptr)
	{
		Subsystem->AddMappingContext(PlayerContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ALKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	ULKInputComponent* LKInputComponent = CastChecked<ULKInputComponent>(InputComponent);
	LKInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALKPlayerController::Move);
	LKInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ALKPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void ALKPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = ThisActor;
	ThisActor = Cast<ILKEnemyInterface>(CursorHit.GetActor());

	if (LastActor == nullptr) // && IsValid(ThisActor))
	{
		if (ThisActor != nullptr)
		{
			ThisActor->HighlightActor();
		}
	}
	else
	{
		if (ThisActor == nullptr)
		{
			LastActor->UnHighlightActor();
		}
		else if (LastActor != ThisActor)
		{
			LastActor->UnHighlightActor();
			ThisActor->HighlightActor();
		}
	}
	
}

void ALKPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{

}

void ALKPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{

}

void ALKPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{

}