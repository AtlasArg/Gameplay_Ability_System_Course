// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LKDamageTextComponent.generated.h"


UCLASS()
class AURA_API ULKDamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(float Damage);
};
