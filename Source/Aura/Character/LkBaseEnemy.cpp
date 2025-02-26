// Copyright Dry Horns 


#include "LkBaseEnemy.h"
#include "Aura/Aura.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/UI/Widget/LKUserWidget.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Aura/AI/LKSAIController.h"
#include "Aura/AbilitySystem/LKAbilitySystemLibrary.h"
#include "Aura/LKGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"


ALkBaseEnemy::ALkBaseEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<ULKAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	// TODO: Check if for LK we need this instead
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<ULKAttributeSet>("AttributeSet");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void ALkBaseEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority())
	{
		return;
	}

	LKSAIController = Cast<ALKSAIController>(NewController);
	LKSAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	LKSAIController->RunBehaviorTree(BehaviorTree);	
	LKSAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	LKSAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), CharacterClass != ECharacterClass::Warrior);
}

void ALkBaseEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);


	UE_LOG(LogTemp, Warning, TEXT("Movement Mode: %d"), (int32)GetCharacterMovement()->MovementMode);
}

void ALkBaseEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 ALkBaseEnemy::GetPlayerLevel()
{
	return Level;
}

void ALkBaseEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	Super::Die();
}

void ALkBaseEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* ALkBaseEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}

void ALkBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		ULKAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}

	ULKUserWidget* LKUSerWidget = Cast<ULKUserWidget>(HealthBar->GetUserWidgetObject());
	if (IsValid(LKUSerWidget))
	{
		LKUSerWidget->SetWidgetController(this);
	}

	const ULKAttributeSet* LKAttributeSet = Cast<ULKAttributeSet>(AttributeSet);
	if (IsValid(LKAttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnHealthChanged.Broadcast(Data.NewValue);
			});

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(LKAttributeSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data) {
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

		
		AbilitySystemComponent->RegisterGameplayTagEvent(FLKGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&ThisClass::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(LKAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(LKAttributeSet->GetMaxHealth());
	}
}

void ALkBaseEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (LKSAIController && LKSAIController->GetBlackboardComponent())
	{
		LKSAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void ALkBaseEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<ULKAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void ALkBaseEnemy::InitializeDefaultAttributes() const
{
	ULKAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}

