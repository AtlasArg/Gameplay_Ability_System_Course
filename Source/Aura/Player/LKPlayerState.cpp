// Copyright Dry Horns 


#include "LKPlayerState.h"

#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Net/UnrealNetwork.h"

ALKPlayerState::ALKPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<ULKAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	// TODO: Check if for LK we need this instead
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<ULKAttributeSet>("AttributeSet");

	//TODO: this may not be needed for LK
	NetUpdateFrequency = 100.f;
}
 
UAbilitySystemComponent* ALKPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ALKPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALKPlayerState, Level);
	DOREPLIFETIME(ALKPlayerState, XP);
}

void ALKPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ALKPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ALKPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ALKPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ALKPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ALKPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}
