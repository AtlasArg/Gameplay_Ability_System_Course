// Copyright Dry Horns 


#include "LKAttributeMenuWidgetController.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/AbilitySystem/Data/AttributeInfo.h"
#include "Aura/LKGameplayTags.h"


void ULKAttributeMenuWidgetController::BroadcastInitialValues()
{
	ULKAttributeSet* AS = CastChecked<ULKAttributeSet>(AttributeSet);
	check(AttributeInfo);
	
	FLKAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FLKGameplayTags::Get().Attributes_Primary_Strength, false);
	Info.AttributeValue = AS->GetStrength();
	 
	AttributeInfoDelegate.Broadcast(Info);
}

void ULKAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}

