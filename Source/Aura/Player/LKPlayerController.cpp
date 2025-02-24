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
#include "Aura/AbilitySystem/LKSTeamAttributeSet.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/Character/LkPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

ALKPlayerController::ALKPlayerController()
{
	// TODO: this may not be needed for LK. cause its not multiplayer.
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	TeamAttributeSet = CreateDefaultSubobject<ULKSTeamAttributeSet>("TeamAttributeSet");
	AbilitySystemComponent = CreateDefaultSubobject<ULKAbilitySystemComponent>("AbilitySystemComponent");
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

void ALKPlayerController::ChangeTeamHealth(float Amount)
{
	if (!TeamAttributeSet) return;

	// Reduce total team health
	float NewHealth = TeamAttributeSet->TeamHealth.GetCurrentValue() + Amount;
	TeamAttributeSet->SetTeamHealth(FMath::Max(0.0f, NewHealth));

	//// Distribute damage proportionally to characters
	//float TotalMaxHealth = TeamAttributeSet->MaxTeamHealth.GetCurrentValue();
	//if (TotalMaxHealth <= 0) return;

	//for (ACharacter* Char : AvailableCharacters)
	//{
	//	if (Char)
	//	{
	//		UAbilitySystemComponent* ASC = Char->GetAbilitySystemComponent();
	//		if (ASC)
	//		{
	//			ULKCharacterAttributeSet* CharAttributes = Cast<ULKCharacterAttributeSet>(ASC->GetAttributeSet(ULKCharacterAttributeSet::StaticClass()));
	//			if (CharAttributes)
	//			{
	//				// Reduce health proportionally
	//				float Proportion = CharAttributes->MaxHealth.GetCurrentValue() / TotalMaxHealth;
	//				float CharNewHealth = CharAttributes->Health.GetCurrentValue() - (DamageAmount * Proportion);
	//				CharAttributes->SetHealth(FMath::Max(0.0f, CharNewHealth));
	//			}
	//		}
	//	}
	//}
}

void ALKPlayerController::InitializeTeamHealth()
{
	if (!TeamAttributeSet) return;

	float TotalHealth = 0.0f;
	float TotalMaxHealth = 0.0f;

	for (ALkCharacterBase* Char : AvailableCharacters)
	{
		if (Char)
		{
			UAbilitySystemComponent* ASC = Char->GetAbilitySystemComponent();
			if (ASC)
			{
				UAttributeSet* Attributes = Char->GetAttributeSet();//ULKAttributeSet::StaticClass());
				if (Attributes)
				{
					ULKAttributeSet* CharAttributes = Cast<ULKAttributeSet>(Attributes);
					//TotalHealth += CharAttributes->Health.GetCurrentValue(); 
					TotalMaxHealth += CharAttributes->MaxHealth.GetCurrentValue(); // solo nos importa el max health por ahora
				}
			}
		}
	}

	TeamAttributeSet->SetTeamHealth(TotalMaxHealth);
	TeamAttributeSet->SetTeamMaxHealth(TotalMaxHealth);

	//TeamAttributeSet->GetTeamMaxHealthAttribute().SetNumericValueChecked(TotalMaxHealth, TeamAttributeSet);
	//TeamAttributeSet->GetTeamHealthAttribute().SetNumericValueChecked(TotalHealth, TeamAttributeSet);
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

	// Find all controlled characters in the world (or spawn them dynamically)
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), FoundCharacters);

	for (AActor* Actor : FoundCharacters)
	{
		ALkPlayerCharacter* Char = Cast<ALkPlayerCharacter>(Actor);
		if (IsValid(Char))  // TODO Gas: check if this is necesary && Char->GetOwner() == this)  // Ensure these belong to this player
		{
			AvailableCharacters.Add(Char);
		}
	}

	// Start with first character
	if (AvailableCharacters.Num() > 0)
	{
		CurrentCharacterIndex = 0;
		Possess(AvailableCharacters[CurrentCharacterIndex]);
	}

	InitializeTeamHealth();
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
	if (InputTag.MatchesTagExact(FLKGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void ALKPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
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

					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
				}

				bAutoRunning = true;
			}
		}

		FollowTime = 0.f;
		bTargeting = false;
	}
}

void ALKPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
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
