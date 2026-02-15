// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class UAuraLoadMenuSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAuraAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	/** Properties */
	UPROPERTY(EditDefaultsOnly, Category="Character Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category="Character Defaults")
	TObjectPtr<UAuraAbilityInfo> AbilityInfo;
	
	UPROPERTY(EditDefaultsOnly) 
	TSubclassOf<USaveGame> LoadMenuSaveGameClass;
	
	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;
	
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;
	
	
	/** Functions */
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	static void DeleteSlotData(const FString& SlotName, int32 SlotIndex);
	UAuraLoadMenuSaveGame* GetSaveSlotData(const FString& SlotName, const int32 SlotIndex) const;
	
};
