// Copyright Dry Horns 


#include "LKAttributeMenuWidgetController.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/AbilitySystem/Data/AttributeInfo.h"
#include "Aura/LKGameplayTags.h"


void ULKAttributeMenuWidgetController::BroadcastInitialValues()
{
	ULKAttributeSet* AS = CastChecked<ULKAttributeSet>(AttributeSet);
	check(AttributeInfo);

	for (auto& Pair : AS->TagsToAttributes)
	{
		FLKAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key, false);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void ULKAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	ULKAttributeSet* AS = CastChecked<ULKAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				/*BroadcastAttributeInfo(Pair.Key, Pair.Value());*/
					FLKAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key, false);
					Info.AttributeValue = Pair.Value().GetNumericValue(AttributeSet);
					AttributeInfoDelegate.Broadcast(Info);

			}
		);
	}
}

//
//void ULKAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
//{
//	FLKAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag, false);
//	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
//	AttributeInfoDelegate.Broadcast(Info);
//}

