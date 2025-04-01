// Copyright Dry Horns 


#include "LKAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Aura/UI/WidgetController/LKWidgetController.h"
#include "Aura/Player/LKPlayerState.h"
#include "Aura/UI/HUD/LKHUD.h"
#include "Aura/Game/LKGameModeBase.h"
#include "Aura/Interaction/LKCombatInterface.h"
#include "AbilitySystemComponent.h"
#include "Aura/LKAbilityTypes.h"

bool ULKAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ALKHUD*& OutAuraHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutAuraHUD = Cast<ALKHUD>(PC->GetHUD());
		if (IsValid(OutAuraHUD))
		{
			ALKPlayerState* PS = PC->GetPlayerState<ALKPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerState = PS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}

	return false;
}

ULKOverlayWidgetController* ULKAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ALKHUD* HUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, HUD))
	{
		return HUD->GetOverlayWidgetController(WCParams);
	}

	return nullptr;
}


ULKAttributeMenuWidgetController* ULKAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ALKHUD* LKHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, LKHUD))
	{
		return LKHUD->GetAttributeMenuWidgetController(WCParams);
	}

	return nullptr;
}

ULKSpellMenuWidgetController* ULKAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ALKHUD* LKHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, LKHUD))
	{
		return LKHUD->GetSpellMenuWidgetController(WCParams);
	}

	return nullptr;
}

void ULKAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{

	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	AActor* AvatarActor = ASC->GetAvatarActor();

	FCharacterClassDefaultInfo ClassDefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
 	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void ULKAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* ClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!IsValid(ClassInfo))
	{
		return;
	}

	for (TSubclassOf<UGameplayAbility> AbilityClass : ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

	const FCharacterClassDefaultInfo& DefaultInfo = ClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<ULKCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, ILKCombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor())); 
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* ULKAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ALKGameModeBase* LKGameMode = Cast<ALKGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (!IsValid(LKGameMode))
	{
		return nullptr;
	}

	return LKGameMode->CharacterClassInfo;
}

ULKAbilityInfo* ULKAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ALKGameModeBase* LKGameMode = Cast<ALKGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (LKGameMode == nullptr) return nullptr;
	return LKGameMode->AbilityInfo;
}

bool ULKAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* AuraEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}

	return false;
}

bool ULKAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* AuraEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}

	return false;
}

void ULKAbilitySystemLibrary::SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FLKGameplayEffectContext* LKEffectContext = static_cast<FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LKEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void ULKAbilitySystemLibrary::SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FLKGameplayEffectContext* LKEffectContext = static_cast<FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LKEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void ULKAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<ULKCombatInterface>() && !ILKCombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ILKCombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool ULKAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

int32 ULKAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return 0;
	}

	const FCharacterClassDefaultInfo& Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}
