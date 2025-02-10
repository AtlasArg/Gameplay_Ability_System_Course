// Copyright Dry Horns 


#include "LKAssetManager.h"
#include "LKGameplayTags.h"
#include "AbilitySystemGlobals.h"

ULKAssetManager& ULKAssetManager::Get()
{
	check(GEngine)
	ULKAssetManager* LKAssetManager = Cast<ULKAssetManager>(GEngine->AssetManager);
	return *LKAssetManager;
}

void ULKAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FLKGameplayTags::InitializeNativeGameplayTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
