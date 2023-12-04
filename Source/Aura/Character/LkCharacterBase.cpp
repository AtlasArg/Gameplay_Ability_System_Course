// Copyright Dry Horns 


#include "LkCharacterBase.h"


ALkCharacterBase::ALkCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	//TODO: Check, not sure about this...
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALkCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}