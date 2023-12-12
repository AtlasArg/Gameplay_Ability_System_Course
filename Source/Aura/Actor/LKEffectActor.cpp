// Copyright Dry Horns 


#include "LKEffectActor.h"
#include "Components/SphereComponent.h"
#include "AbilitySystemInterface.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"

ALKEffectActor::ALKEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void ALKEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ALKEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ALKEffectActor::OnEndOverlap);
}

void ALKEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// TODO: change this to apply a gameplay effect. Using a const_Cast as a hack
	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor);
	if (ASCInterface != nullptr)
	{
		const ULKAttributeSet* AttributeSet = Cast<ULKAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(ULKAttributeSet::StaticClass()));
		ULKAttributeSet* MutableAttributeSet = const_cast<ULKAttributeSet*>(AttributeSet);
		MutableAttributeSet->SetHealth(AttributeSet->GetHealth() + 25.f);
		Destroy();
	}
}

void ALKEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

