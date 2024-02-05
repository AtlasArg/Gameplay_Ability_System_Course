// Copyright Dry Horns 


#include "LKAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Aura/UI/WidgetController/LKWidgetController.h"
#include "Aura/Player/LKPlayerState.h"
#include "Aura/UI/HUD/LKHUD.h"

ULKOverlayWidgetController* ULKAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ALKHUD* HUD = Cast<ALKHUD>(PC->GetHUD()))
		{
			ALKPlayerState* PS = PC->GetPlayerState<ALKPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return HUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}


ULKAttributeMenuWidgetController* ULKAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ALKHUD* HUD = Cast<ALKHUD>(PC->GetHUD()))
		{
			ALKPlayerState* PS = PC->GetPlayerState<ALKPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return HUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}