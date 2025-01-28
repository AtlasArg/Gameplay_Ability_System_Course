// Copyright Dry Horns 


#include "LKProjectileSpell.h"
#include "Aura/Actor/LKProjectile.h"
#include "Aura/Interaction/LKCombatInterface.h"

void ULKProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// TODO: this is no needed in LK
	if (HasAuthority(&ActivationInfo))
	{
		ILKCombatInterface* CombatInterface = Cast<ILKCombatInterface>(GetAvatarActorFromActorInfo());
		if (CombatInterface)
		{
			const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SocketLocation);

			ALKProjectile* Projectile = GetWorld()->SpawnActorDeferred<ALKProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


			Projectile->FinishSpawning(SpawnTransform);
		}
	}
}
