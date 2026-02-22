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
struct FSavedActor
{
	GENERATED_BODY()
	
	UPROPERTY()
	FName ActorName = FName();
	
	UPROPERTY()
	FTransform ActorTransform = FTransform();
	
	// Serialized variables from the Actor - only those marked with SaveGame specifier
	UPROPERTY()
	TArray<uint8> Bytes;
};

// add the AddUnique capabilities
inline bool operator==(const FSavedActor& Left, const FSavedActor& Right) { return Left.ActorName == Right.ActorName; }




USTRUCT(BlueprintType)
struct FSavedMap
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString MapAssetName = FString();
	
	UPROPERTY()
	TArray<FSavedActor> SavedActors;
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

// Add the AddUnique capabilities
inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right) { return Left.AbilityTag.MatchesTagExact(Right.AbilityTag); }


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
	UPROPERTY() FString MapAssetName = FString("Default Map Asset Name");
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
	
	/** Actor Datas Per Map */
	UPROPERTY() TArray<FSavedMap> SavedMaps;
	
	
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
	
};
