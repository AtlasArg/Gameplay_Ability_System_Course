// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LKHUD.generated.h"

class ULKUserWidget;

UCLASS()
class AURA_API ALKHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ULKUserWidget> OverlayWidget;

protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULKUserWidget> OverlayWidgetClass;
};
