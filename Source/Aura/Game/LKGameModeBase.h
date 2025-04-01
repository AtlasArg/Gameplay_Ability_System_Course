// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LKGameModeBase.generated.h"

class UCharacterClassInfo;
class ULKAbilityInfo;

UCLASS()
class AURA_API ALKGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Ability Info")
	TObjectPtr<ULKAbilityInfo> AbilityInfo;
};
