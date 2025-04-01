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
	DOREPLIFETIME(ALKPlayerState, AttributePoints);
	DOREPLIFETIME(ALKPlayerState, SpellPoints);
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

void ALKPlayerState::AddToAttributePoints(int32 InPoints)
{
	AttributePoints += InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ALKPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
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

void ALKPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void ALKPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
