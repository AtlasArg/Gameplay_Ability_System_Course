// Copyright Dry Horns 


#include "LKOverlayWidgetController.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"

void ULKOverlayWidgetController::BroadcastInitialValues()
{
	const ULKAttributeSet* LKAttributeSet = CastChecked<ULKAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(LKAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(LKAttributeSet->GetMaxHealth());

	OnManaChanged.Broadcast(LKAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(LKAttributeSet->GetMaxMana());
}

void ULKOverlayWidgetController::BindCallbacksToDependencies()
{
	const ULKAttributeSet* LKAttributeSet = CastChecked<ULKAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetHealthAttribute()).AddUObject(this, &ULKOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &ULKOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetManaAttribute()).AddUObject(this, &ULKOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetMaxManaAttribute()).AddUObject(this, &ULKOverlayWidgetController::MaxManaChanged);
}

void ULKOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void ULKOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void ULKOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void ULKOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}