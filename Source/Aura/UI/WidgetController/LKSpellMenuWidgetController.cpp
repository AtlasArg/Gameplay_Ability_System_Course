// Copyright Dry Horns 


#include "LKSpellMenuWidgetController.h"
#include "Aura/AbilitySystem/Data/LKAbilityInfo.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"

void ULKSpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
}

void ULKSpellMenuWidgetController::BindCallbacksToDependencies()
{
    GetLKASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
        {
            if (AbilityInfo)
            {
                FLKSAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
                Info.StatusTag = StatusTag;
                AbilityInfoDelegate.Broadcast(Info);
            }
        });
}
