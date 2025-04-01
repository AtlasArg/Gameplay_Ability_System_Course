// Copyright Dry Horns 


#include "LKWidgetController.h"
#include "Aura/Player/LKPlayerController.h"
#include "Aura/Player/LKPlayerState.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/AbilitySystem/Data/LKAbilityInfo.h"

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

void ULKWidgetController::BroadcastAbilityInfo()
{
	if (!GetLKASC()->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FLKSAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(LKAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = LKAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = LKAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});
	GetLKASC()->ForEachAbility(BroadcastDelegate);
}

ALKPlayerController* ULKWidgetController::GetLKPC()
{
	if (LKPlayerController == nullptr)
	{
		LKPlayerController = Cast<ALKPlayerController>(PlayerController);
	}
	return LKPlayerController;
}

ALKPlayerState* ULKWidgetController::GetLKPS()
{
	if (LKPlayerState == nullptr)
	{
		LKPlayerState = Cast<ALKPlayerState>(PlayerState);
	}
	return LKPlayerState;
}

ULKAbilitySystemComponent* ULKWidgetController::GetLKASC()
{
	if (LKAbilitySystemComponent == nullptr)
	{
		LKAbilitySystemComponent = Cast<ULKAbilitySystemComponent>(AbilitySystemComponent);
	}
	return LKAbilitySystemComponent;
}

ULKAttributeSet* ULKWidgetController::GetLKAS()
{
	if (LKAttributeSet == nullptr)
	{
		LKAttributeSet = Cast<ULKAttributeSet>(AttributeSet);
	}
	return LKAttributeSet;
}
