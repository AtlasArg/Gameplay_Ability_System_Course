// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "LkCharacterBase.h"
#include "Aura/Interaction/LKEnemyInterface.h"
#include "Aura/UI/WidgetController/LKOverlayWidgetController.h"
#include "Aura/AbilitySystem/Data/CharacterClassInfo.h"
#include "LkBaseEnemy.generated.h"

class UWidgetComponent;

UCLASS()
class AURA_API ALkBaseEnemy : public ALkCharacterBase, public ILKEnemyInterface
{
	GENERATED_BODY()
	
public:
	ALkBaseEnemy();

	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UFUNCTION()
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;
};
