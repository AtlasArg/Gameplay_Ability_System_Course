// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "Aura/AbilitySystem/Abilities/LKProjectileSpell.h"
#include "LKFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULKFireBolt : public ULKProjectileSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

    UFUNCTION(BlueprintCallable)
    void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget);

protected:

    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    float ProjectileSpread = 90.f;

    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    int32 MaxNumProjectiles = 5;

    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    float HomingAccelerationMin = 1600.f;

    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    float HomingAccelerationMax = 3200.f;

    UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
    bool bLaunchHomingProjectiles = true;
};
