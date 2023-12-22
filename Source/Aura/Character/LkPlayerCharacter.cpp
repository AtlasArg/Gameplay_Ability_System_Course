// Copyright Dry Horns 


#include "LkPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/Player/LKPlayerState.h"
#include "Aura/Player/LKPlayerController.h"
#include "Aura/UI/HUD/LKHUD.h"

ALkPlayerCharacter::ALkPlayerCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = RotationRate;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ALkPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void ALkPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

int32 ALkPlayerCharacter::GetPlayerLevel()
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	return LKPlayerState->GetPlayerLevel();
}

void ALkPlayerCharacter::InitAbilityActorInfo()
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	LKPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(LKPlayerState, this);
	Cast<ULKAbilitySystemComponent>(LKPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

	AbilitySystemComponent = LKPlayerState->GetAbilitySystemComponent();
	AttributeSet = LKPlayerState->GetAttributeSet();

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
