// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "Aura/UI/WidgetController/LKWidgetController.h"
#include "LKSpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API ULKSpellMenuWidgetController : public ULKWidgetController
{
	GENERATED_BODY()
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
};
