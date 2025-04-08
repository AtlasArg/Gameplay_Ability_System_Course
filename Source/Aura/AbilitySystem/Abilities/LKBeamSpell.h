// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "Aura/AbilitySystem/Abilities/LKDamageGameplayAbility.h"
#include "LKBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULKBeamSpell : public ULKDamageGameplayAbility
{
	GENERATED_BODY()
public:

    UFUNCTION(BlueprintCallable)
    void StoreMouseDataInfo(const FHitResult& HitResult);

    UFUNCTION(BlueprintCallable)
    void StoreOwnerVariables();

    UFUNCTION(BlueprintCallable)
    void TraceFirstTarget(const FVector& BeamTargetLocation);

    UFUNCTION(BlueprintCallable)
    void StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets);
protected:

    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    FVector MouseHitLocation;

    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    TObjectPtr<AActor> MouseHitActor;

    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    TObjectPtr<APlayerController> OwnerPlayerController;

    UPROPERTY(BlueprintReadWrite, Category = "Beam")
    TObjectPtr<ACharacter> OwnerCharacter;

    UPROPERTY(EditDefaultsOnly, Category = "Beam")
    int32 MaxNumShockTargets = 5;
};
