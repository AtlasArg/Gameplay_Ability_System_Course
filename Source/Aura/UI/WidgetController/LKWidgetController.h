// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LKWidgetController.generated.h"

class APlayerController;
class APlayerState;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class AURA_API ULKWidgetController : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
