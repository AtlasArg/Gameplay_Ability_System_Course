// Copyright Dry Horns 


#include "LKHUD.h"
#include "Aura/UI/Widget/LKUserWidget.h"

void ALKHUD::BeginPlay()
{
	Super::BeginPlay();
	
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	Widget->AddToViewport();
}
