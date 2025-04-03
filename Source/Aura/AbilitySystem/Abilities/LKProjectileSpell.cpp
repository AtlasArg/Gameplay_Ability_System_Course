// Copyright Dry Horns 


#include "LKProjectileSpell.h"
#include "Aura/Actor/LKProjectile.h"
#include "Aura/Interaction/LKCombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Aura/LKGameplayTags.h"

void ULKProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void ULKProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) { return; }

	const FVector SocketLocation = ILKCombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		FLKGameplayTags::Get().Montage_Attack_Weapon);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	ALKProjectile* Projectile = GetWorld()->SpawnActorDeferred<ALKProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Projectile->FinishSpawning(SpawnTransform);
}
