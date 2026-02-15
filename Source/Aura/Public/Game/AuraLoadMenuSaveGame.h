// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AuraLoadMenuSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraLoadMenuSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	
	UPROPERTY() FString SlotName = FString();
	UPROPERTY() int32 SlotIndex = 0;
	UPROPERTY() FString PlayerName = FString("Default");
	
};
