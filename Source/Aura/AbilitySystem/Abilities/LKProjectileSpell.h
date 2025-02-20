// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "LKDamageGameplayAbility.h"
#include "LKProjectileSpell.generated.h"

class ALKProjectile;
class UGameplayEffect;

UCLASS()
class AURA_API ULKProjectileSpell : public ULKDamageGameplayAbility
{
	GENERATED_BODY()

public:

protected:

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ALKProjectile> ProjectileClass;

};
