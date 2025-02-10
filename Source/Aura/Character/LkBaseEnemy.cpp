// Copyright Dry Horns 


#include "LkBaseEnemy.h"
#include "Aura/Aura.h"
#include "Aura/AbilitySystem/LKAbilitySystemComponent.h"
#include "Aura/AbilitySystem/LKAttributeSet.h"
#include "Aura/UI/Widget/LKUserWidget.h"
#include "Components/WidgetComponent.h"


ALkBaseEnemy::ALkBaseEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<ULKAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	// TODO: Check if for LK we need this instead
	//AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<ULKAttributeSet>("AttributeSet");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void ALkBaseEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
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

void ALkBaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

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

		OnHealthChanged.Broadcast(LKAttributeSet->GetHealth());
		OnMaxHealthChanged.Broadcast(LKAttributeSet->GetMaxHealth());
	}
}

void ALkBaseEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<ULKAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAttributes();
}

