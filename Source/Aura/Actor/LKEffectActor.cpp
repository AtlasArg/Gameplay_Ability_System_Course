// Copyright Dry Horns 


#include "LKEffectActor.h"
//#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

ALKEffectActor::ALKEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void ALKEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	/*Sphere->OnComponentBeginOverlap.AddDynamic(this, &ALKEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ALKEffectActor::OnEndOverlap);*/
}

void ALKEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetASC != nullptr)
	{
		check(GameplayEffectClass);
		FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
		EffectContextHandle.AddSourceObject(this);
		FGameplayEffectSpecHandle EffectSpectHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContextHandle);

		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpectHandle.Data.Get());
	}
}

//void ALKEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	// TODO: change this to apply a gameplay effect. Using a const_Cast as a hack
//	IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor);
//	if (ASCInterface != nullptr)
//	{
//		const ULKAttributeSet* AttributeSet = Cast<ULKAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(ULKAttributeSet::StaticClass()));
//		ULKAttributeSet* MutableAttributeSet = const_cast<ULKAttributeSet*>(AttributeSet);
//		MutableAttributeSet->SetHealth(AttributeSet->GetHealth() + 25.f);
//		MutableAttributeSet->SetMana(AttributeSet->GetMana() - 25.f);
//		Destroy();
//	}
//}
//
//void ALKEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//
//}

