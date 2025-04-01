// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "Aura/UI/WidgetController/LKWidgetController.h"
#include "LKAttributeMenuWidgetController.generated.h"

struct FLKAttributeInfo;
class UAttributeInfo;
struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FLKAttributeInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API ULKAttributeMenuWidgetController : public ULKWidgetController
{
	GENERATED_BODY()
public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnPlayerStatChangedSignature AttributePointsChangedDelegate;

	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);

protected:


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
//
//private:
//
//	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
