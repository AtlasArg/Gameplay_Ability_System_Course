// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LKEffectActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class AURA_API ALKEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ALKEffectActor();

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(Visibleanywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(Visibleanywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

};
