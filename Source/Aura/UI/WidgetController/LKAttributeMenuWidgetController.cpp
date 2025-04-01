// Copyright Dry Horns 


#include "LKAttributeMenuWidgetController.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/AbilitySystem/Data/AttributeInfo.h"
#include "Aura/LKGameplayTags.h"
#include "Aura/Player/LKPlayerState.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"

void ULKAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AttributeInfo);

	for (auto& Pair : GetLKAS()->TagsToAttributes)
	{
		FLKAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key, false);
		Info.AttributeValue = Pair.Value().GetNumericValue(GetLKAS());
		AttributeInfoDelegate.Broadcast(Info);
	}

	AttributePointsChangedDelegate.Broadcast(GetLKPS()->GetAttributePoints());
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
	
	GetLKPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
}

void ULKAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	ULKAbilitySystemComponent* AuraASC = CastChecked<ULKAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
}


//void ULKAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
//{
//	FLKAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag, false);
//	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
//	AttributeInfoDelegate.Broadcast(Info);
//}

