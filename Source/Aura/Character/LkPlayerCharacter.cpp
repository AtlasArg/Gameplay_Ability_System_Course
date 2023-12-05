// Copyright Dry Horns 


#include "LkPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/Player/LKPlayerState.h"

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

void ALkPlayerCharacter::InitAbilityActorInfo()
{
	ALKPlayerState* LKPlayerState = GetPlayerState<ALKPlayerState>();
	check(LKPlayerState);
	LKPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(LKPlayerState, this);
	AbilitySystemComponent = LKPlayerState->GetAbilitySystemComponent();
	AttributeSet = LKPlayerState->GetAttributeSet();
}
