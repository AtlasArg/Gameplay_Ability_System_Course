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
#include "Aura/LKGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"

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

bool ULKAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* LKEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return LKEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float ULKAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* LKEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return LKEffectContext->GetDebuffDamage();
	}
	return 0.f;
}

float ULKAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* LKEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return LKEffectContext->GetDebuffDuration();
	}
	return 0.f;
}

float ULKAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* LKEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return LKEffectContext->GetDebuffFrequency();
	}
	return 0.f;
}

FGameplayTag ULKAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* LKEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (LKEffectContext->GetDamageType().IsValid())
		{
			return *LKEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector ULKAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* LKEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return LKEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector ULKAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* LKEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return LKEffectContext->GetKnockbackForce();
	}
	return FVector::ZeroVector;
}

bool ULKAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FLKGameplayEffectContext* LKEffectContext = static_cast<const FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return LKEffectContext->IsCriticalHit();
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

void ULKAbilitySystemLibrary::SetIsSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInSuccessfulDebuff)
{
	if (FLKGameplayEffectContext* LKEffectContext = static_cast<FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LKEffectContext->SetIsSuccessfulDebuff(bInSuccessfulDebuff);
	}
}

void ULKAbilitySystemLibrary::SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FLKGameplayEffectContext* LKEffectContext = static_cast<FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LKEffectContext->SetDebuffDamage(InDamage);
	}
}

void ULKAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDuration)
{
	if (FLKGameplayEffectContext* LKEffectContext = static_cast<FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LKEffectContext->SetDebuffDuration(InDuration);
	}
}

void ULKAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InFrequency)
{
	if (FLKGameplayEffectContext* LKEffectContext = static_cast<FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LKEffectContext->SetDebuffFrequency(InFrequency);
	}
}

void ULKAbilitySystemLibrary::SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FLKGameplayEffectContext* LKEffectContext = static_cast<FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		LKEffectContext->SetDamageType(DamageType);
	}
}

void ULKAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
	if (FLKGameplayEffectContext* LKEffectContext = static_cast<FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LKEffectContext->SetDeathImpulse(InImpulse);
	}
}

void ULKAbilitySystemLibrary::SetKnockbackForce(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FLKGameplayEffectContext* LKEffectContext = static_cast<FLKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		LKEffectContext->SetKnockbackForce(InForce);
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

void ULKAbilitySystemLibrary::GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTargets)
	{
		OutClosestTargets = Actors;
		return;
	}

	TArray<AActor*> ActorsToCheck = Actors;
	int32 NumTargetsFound = 0;

	while (NumTargetsFound < MaxTargets)
	{
		if (ActorsToCheck.Num() == 0) break;
		double ClosestDistance = TNumericLimits<double>::Max();
		AActor* ClosestActor;
		for (AActor* PotentialTarget : ActorsToCheck)
		{
			const double Distance = (PotentialTarget->GetActorLocation() - Origin).Length();
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = PotentialTarget;
			}
		}
		ActorsToCheck.Remove(ClosestActor);
		OutClosestTargets.AddUnique(ClosestActor);
		++NumTargetsFound;
	}
}

bool ULKAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriends = bBothArePlayers || bBothAreEnemies;
	return !bFriends;
}

FGameplayEffectContextHandle ULKAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FLKGameplayTags& GameplayTags = FLKGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	FGameplayEffectContextHandle EffectContexthandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();
	EffectContexthandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContexthandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContexthandle, DamageEffectParams.KnockbackForce);
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGameplayEffectClass, DamageEffectParams.AbilityLevel, EffectContexthandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContexthandle;
}

TArray<FRotator> ULKAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	const float DeltaSpread = Spread / (NumRotators - 1);

	if (NumRotators > 1)
	{
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}

	return Rotators;
}

TArray<FVector> ULKAbilitySystemLibrary::EvenlyRotatedVectors(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	const float DeltaSpread = Spread / (NumVectors - 1);

	if (NumVectors > 1)
	{
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}

	return Vectors;
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
