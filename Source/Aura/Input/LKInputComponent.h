// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Aura/Input/LKInputConfig.h"
#include "LKInputComponent.generated.h"

UCLASS()
class AURA_API ULKInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	/*template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const ULKIputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);*/

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const ULKInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void ULKInputComponent::BindAbilityActions(const ULKInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, 
	ReleasedFuncType ReleasedFunc, 
	HeldFuncType HeldFunc)
{
	check(InputConfig);

	for (const FLKInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);
			}

			if (ReleasedFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);
			}

			if (HeldFunc)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
			}
		}
	}
}
