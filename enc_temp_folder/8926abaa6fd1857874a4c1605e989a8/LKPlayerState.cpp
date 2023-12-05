// Copyright Dry Horns 


#include "LKPlayerState.h"

#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"

ALKPlayerState::ALKPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<ULKAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AttributeSet = CreateDefaultSubobject<ULKAttributeSet>("AttributeSet");

	//TODO: this may not be needed for LK
	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ALKPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
