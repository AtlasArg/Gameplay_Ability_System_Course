// Copyright Dry Horns 


#include "LKUserWidget.h"

void ULKUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
