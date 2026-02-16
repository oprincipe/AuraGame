// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AuraLoadMenuSaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

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
	UPROPERTY() FString MapName = FString("Default Map Name");
	UPROPERTY() TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;
	UPROPERTY() FName PlayerStartTag = NAME_None;
	UPROPERTY() bool bFirstTimeLoading = true;
	
	/** Player Values */
	UPROPERTY() int32 PlayerLevel = 0;
	UPROPERTY() int32 XP = 0;
	UPROPERTY() int32 SpellPoints = 0;
	UPROPERTY() int32 AttributePoints = 0;
	
	/** Attributes */
	UPROPERTY() float Strength = 0.f;
	UPROPERTY() float Intelligence = 0.f;
	UPROPERTY() float Resilience = 0.f;
	UPROPERTY() float Vigor = 0.f;
};
