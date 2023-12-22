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
}

void ALKPlayerState::OnRep_Level(int32 OldLevel)
{

}
