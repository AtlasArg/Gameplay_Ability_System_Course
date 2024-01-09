// Copyright Dry Horns 

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "LKAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULKAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static ULKAssetManager& Get();
protected:
	virtual void StartInitialLoading() override;

};
