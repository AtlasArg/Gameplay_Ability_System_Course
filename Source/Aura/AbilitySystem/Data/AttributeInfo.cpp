// Copyright Dry Horns 


#include "AttributeInfo.h"
#include "Aura/LKSLogChannels.h"

FLKAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FLKAttributeInfo Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for attributertag [%s] on attributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FLKAttributeInfo();
}
