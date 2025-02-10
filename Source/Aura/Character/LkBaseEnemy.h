// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "LkCharacterBase.h"
#include "Aura/Interaction/LKEnemyInterface.h"
#include "Aura/UI/WidgetController/LKOverlayWidgetController.h"
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

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

};
