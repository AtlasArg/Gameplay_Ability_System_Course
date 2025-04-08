// Copyright Dry Horns 


#include "LKPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Aura/Input/LKInputComponent.h"
#include "Aura/Input/LKInputConfig.h"
#include "Aura/Interaction/LKEnemyInterface.h"
#include "Aura/LKGameplayTags.h"
#include "Components/SplineComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Aura/UI/Widget/LKDamageTextComponent.h"
#include "GameFramework/Character.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"

ALKPlayerController::ALKPlayerController()
{
	// TODO: this may not be needed for LK. cause its not multiplayer.
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ALKPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();

	AutoRun();
}

void ALKPlayerController::ShowDamageNumber_Implementation(float Damage, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		ULKDamageTextComponent* DamageText = NewObject<ULKDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(Damage, bBlockedHit, bCriticalHit);
	}
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
	LKInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ALKPlayerController::ShiftPressed);
	LKInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ALKPlayerController::ShiftReleased);
	LKInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void ALKPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FLKGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}

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
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FLKGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (ThisActor)
		{
			ThisActor->UnHighlightActor();
		}
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}

	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}

	LastActor = ThisActor;
	ThisActor = Cast<ILKEnemyInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (ThisActor)
		{
			ThisActor->HighlightActor();
		}
	}
}

void ALKPlayerController::AutoRun()
{
	if (!bAutoRunning)
	{
		return;
	}

	APawn* ControlledPawn = GetPawn();
	if (IsValid(ControlledPawn))
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAccpetanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ALKPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FLKGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}
	if (InputTag.MatchesTagExact(FLKGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
	if (GetASC())
	{
		GetASC()->AbilityInputTagPressed(InputTag);
	}
}

void ALKPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FLKGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(FLKGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}

		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}

	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreashold && IsValid(ControlledPawn))
		{
			UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination);
			if (NavPath)
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}

				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
			if (GetASC() && !GetASC()->HasMatchingGameplayTag(FLKGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
		}

		FollowTime = 0.f;
		bTargeting = false;
	}
}

void ALKPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FLKGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	if (!InputTag.MatchesTagExact(FLKGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}

		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint; // we can use location also.
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
}

ULKAbilitySystemComponent* ALKPlayerController::GetASC()
{
	if (!IsValid(AbilitySystemComponent))// = nullptr)
	{
		AbilitySystemComponent = Cast<ULKAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return AbilitySystemComponent;
}
