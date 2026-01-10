// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(const int32 InXP);
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};
