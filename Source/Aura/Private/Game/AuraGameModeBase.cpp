// Copyright NattySoft


#include "Game/AuraGameModeBase.h"

#include "Game/AuraLoadMenuSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}
	
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	UAuraLoadMenuSaveGame* LoadMenuSaveGame = Cast<UAuraLoadMenuSaveGame>(SaveGameObject);
	LoadMenuSaveGame->PlayerName = LoadSlot->PlayerName;
	LoadMenuSaveGame->SlotIndex = SlotIndex;
	
	UGameplayStatics::SaveGameToSlot(LoadMenuSaveGame, LoadSlot->LoadSlotName, SlotIndex);
	
}
