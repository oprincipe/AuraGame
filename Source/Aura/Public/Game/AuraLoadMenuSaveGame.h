// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "AuraLoadMenuSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityTag = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityStatus = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilitySlot = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	FGameplayTag AbilityType = FGameplayTag();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="ClassDefaults")
	int32 AbilityLevel = 0;
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
	UPROPERTY() int32 PlayerLevel = 1;
	UPROPERTY() int32 XP = 0;
	UPROPERTY() int32 SpellPoints = 0;
	UPROPERTY() int32 AttributePoints = 0;
	
	/** Attributes */
	UPROPERTY() float Strength = 0.f;
	UPROPERTY() float Intelligence = 0.f;
	UPROPERTY() float Resilience = 0.f;
	UPROPERTY() float Vigor = 0.f;
	
	/** Abilities */
	UPROPERTY() TArray<FSavedAbility> SavedAbilities;
};
