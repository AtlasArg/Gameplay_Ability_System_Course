// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LKAbilitySystemLibrary.generated.h"

class ULKOverlayWidgetController;
class ULKAttributeMenuWidgetController;

UCLASS()
class AURA_API ULKAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "LKAbilitySystemLibrary|WidgetController")
	static ULKOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);


	UFUNCTION(BlueprintPure, Category = "LKAbilitySystemLibrary|WidgetController")
	static ULKAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
};
