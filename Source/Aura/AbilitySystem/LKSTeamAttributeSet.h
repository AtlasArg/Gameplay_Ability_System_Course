// Cirtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;opyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "LKSTeamAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class AURA_API ULKSTeamAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData TeamHealth;
    ATTRIBUTE_ACCESSORS(ULKSTeamAttributeSet, TeamHealth);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData TeamMaxHealth;
    ATTRIBUTE_ACCESSORS(ULKSTeamAttributeSet, TeamMaxHealth);
};
