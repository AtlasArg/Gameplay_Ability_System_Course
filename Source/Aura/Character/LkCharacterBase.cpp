// Copyright Dry Horns 


#include "LkCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Aura/LKGameplayTags.h"
#include "Aura/Aura.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


ALkCharacterBase::ALkCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	const FLKGameplayTags& GameplayTags = FLKGameplayTags::Get();

	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = GameplayTags.Debuff_Burn;

	StunDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = GameplayTags.Debuff_Stun;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	//TODO: Check, not sure about this...
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ALkCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALkCharacterBase, bIsStunned);
	DOREPLIFETIME(ALkCharacterBase, bIsBurned);
	DOREPLIFETIME(ALkCharacterBase, bIsBeingShocked);
}

void ALkCharacterBase::OnRep_Stunned()
{
}

void ALkCharacterBase::OnRep_Burned()
{
}

void ALkCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

FVector ALkCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FLKGameplayTags& GameplayTags = FLKGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.Montage_Attack_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	return FVector();
}

void ALkCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void ALkCharacterBase::InitAbilityActorInfo()
{

}

void ALkCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1);
	ApplyEffectToSelf(DefaultVitalAttributes, 1);
}

void ALkCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ALkCharacterBase::AddCharacterAbilities()
{
	ULKAbilitySystemComponent* ASC = CastChecked<ULKAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority())
	{
		return;
	}

	ASC->AddCharacterAbilites(StartupAbilities); 
	ASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}

void ALkCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}

UAbilitySystemComponent* ALkCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ALkCharacterBase::GetAttributeSet() const
{
	return AttributeSet;
}

UAnimMontage* ALkCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void ALkCharacterBase::Die(const FVector& DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath(DeathImpulse);
}

FOnDeathSignature& ALkCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

bool ALkCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* ALkCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> ALkCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

ECharacterClass ALkCharacterBase::GetCharacterClass_Implementation()
{
	return CharacterClass;
}

FOnASCRegistered& ALkCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnAscRegistered;
}

USkeletalMeshComponent* ALkCharacterBase::GetWeapon_Implementation()
{
	return Weapon;
}

void ALkCharacterBase::SetIsBeingShocked_Implementation(bool bInShock)
{
	bIsBeingShocked = bInShock;
}

bool ALkCharacterBase::IsBeingShocked_Implementation() const
{
	return bIsBeingShocked;
}

void ALkCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Dissolve();
	bDead = true;
	BurnDebuffComponent->Deactivate();
	OnDeathDelegate.Broadcast(this);
}
