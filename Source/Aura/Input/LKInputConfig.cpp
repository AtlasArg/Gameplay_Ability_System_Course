// Copyright Dry Horns 


#include "LKInputConfig.h"

const UInputAction* ULKInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const FLKInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction != nullptr && Action.InputTag.MatchesTagExact(InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find AbilityInputAction for InputTag[%s], on InputConfig[%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}