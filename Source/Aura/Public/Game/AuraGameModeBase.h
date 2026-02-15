// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

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
	
public:
	UPROPERTY(EditDefaultsOnly, Category="Character Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	
	UPROPERTY(EditDefaultsOnly, Category="Character Defaults")
	TObjectPtr<UAuraAbilityInfo> AbilityInfo;
	
	
	UPROPERTY(EditDefaultsOnly) TSubclassOf<USaveGame> LoadMenuSaveGameClass;
	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	
};
