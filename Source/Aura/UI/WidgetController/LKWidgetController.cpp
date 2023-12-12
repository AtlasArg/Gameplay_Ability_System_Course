// Copyright Dry Horns 


#include "LKWidgetController.h"

void ULKWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void ULKWidgetController::BroadcastInitialValues()
{

}

void ULKWidgetController::BindCallbacksToDependencies()
{
}
