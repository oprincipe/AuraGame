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
	int32 GetXP() const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(const int32 InLevel) const;
	
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(const int32 InLevel) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(const int32 InPlayerLevel);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(const int32 InAttributePoints);
	
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(const int32 InSpellPoints);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(const int32 InXP) const;
	
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();
};
