// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LKHUD.generated.h"

class ULKUserWidget;
class ULKOverlayWidgetController;
struct FWidgetControllerParams;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class AURA_API ALKHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<ULKUserWidget> OverlayWidget;

	ULKOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULKUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<ULKOverlayWidgetController> OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULKOverlayWidgetController> OverlayWidgetControllerClass;
};
