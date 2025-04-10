// Copyright Dry Horns 


#include "LkPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/Player/LKPlayerState.h"
#include "NiagaraComponent.h"
#include "Aura/AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Aura/Player/LKPlayerController.h"
#include "Aura/UI/HUD/LKHUD.h"
#include "Aura/AbilitySystem/Data/LevelUpInfo.h"
#include "Aura/LKGameplayTags.h"

ALkPlayerCharacter::ALkPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = RotationRate;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void ALkPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void ALkPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void ALkPlayerCharacter::AddToXP_Implementation(int32 InXP)
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	LKPlayerState->AddToXP(InXP);
}

void ALkPlayerCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

int32 ALkPlayerCharacter::GetXP_Implementation() const
{
	const ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	return LKPlayerState->GetXP();
}

int32 ALkPlayerCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	return LKPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ALkPlayerCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	return LKPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 ALkPlayerCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	return LKPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void ALkPlayerCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	LKPlayerState->AddToLevel(InPlayerLevel);

	if (ULKAbilitySystemComponent* LKASC = Cast<ULKAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		LKASC->UpdateAbilityStatuses(LKPlayerState->GetPlayerLevel());
	}
}

void ALkPlayerCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	LKPlayerState->AddToAttributePoints(InAttributePoints);
}

void ALkPlayerCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	LKPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 ALkPlayerCharacter::GetAttributePoints_Implementation() const
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	return LKPlayerState->GetAttributePoints();
}

int32 ALkPlayerCharacter::GetSpellPoints_Implementation() const
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	return LKPlayerState->GetSpellPoints();
}

int32 ALkPlayerCharacter::GetPlayerLevel_Implementation()
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	return LKPlayerState->GetPlayerLevel();
}

void ALkPlayerCharacter::OnRep_Stunned()
{
	if (ULKAbilitySystemComponent* LKASC = Cast<ULKAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FLKGameplayTags& GameplayTags = FLKGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			LKASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			LKASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void ALkPlayerCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

void ALkPlayerCharacter::InitAbilityActorInfo()
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	LKPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(LKPlayerState, this);
	Cast<ULKAbilitySystemComponent>(LKPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = LKPlayerState->GetAbilitySystemComponent();
	AttributeSet = LKPlayerState->GetAttributeSet();
	OnAscRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FLKGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);

	ALKPlayerController* LKPlayerController = Cast<ALKPlayerController>(GetController());
	if (LKPlayerController != nullptr)
	{
		ALKHUD* LKHUD = Cast<ALKHUD>(LKPlayerController->GetHUD());
		if (LKHUD != nullptr)
		{
			LKHUD->InitOverlay(LKPlayerController, LKPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}

void ALkPlayerCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}
