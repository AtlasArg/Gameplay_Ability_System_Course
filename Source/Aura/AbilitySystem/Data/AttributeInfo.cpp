// Copyright Dry Horns 


#include "AttributeInfo.h"

//FLKAttributeInfo ULKAttributeInfoData::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const
//{
//	for (const FLKAttributeInfo Info: AttributeInformation)
//	{
//		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
//		{
//			return Info;
//		}
//	}
//
//	if (bLogNotFound)
//	{
//		UE_LOG(LogTemp, Error, TEXT("Can't find info for attributertag [%s] on attributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
//	}
//
//	return FLKAttributeInfo();
//}

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
		UE_LOG(LogTemp, Error, TEXT("Can't find info for attributertag [%s] on attributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FLKAttributeInfo();
}
