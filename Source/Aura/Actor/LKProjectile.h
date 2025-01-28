// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LKProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class AURA_API ALKProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ALKProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr <UProjectileMovementComponent> ProjectileMovement;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResults);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
