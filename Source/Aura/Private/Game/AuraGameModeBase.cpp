// Copyright NattySoft


#include "Game/AuraGameModeBase.h"

#include "Game/AuraLoadMenuSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	DeleteSlotData(LoadSlot->LoadSlotName, SlotIndex);
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	UAuraLoadMenuSaveGame* LoadMenuSaveGame = Cast<UAuraLoadMenuSaveGame>(SaveGameObject);
	LoadMenuSaveGame->PlayerName = LoadSlot->PlayerName;
	LoadMenuSaveGame->SlotIndex = SlotIndex;
	LoadMenuSaveGame->SaveSlotStatus = Taken;
	
	UGameplayStatics::SaveGameToSlot(LoadMenuSaveGame, LoadSlot->LoadSlotName, SlotIndex);
	
}

void AAuraGameModeBase::DeleteSlotData(const FString& SlotName, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

UAuraLoadMenuSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, const int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		 SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadMenuSaveGameClass);
	}
	
	UAuraLoadMenuSaveGame* LoadMenuSaveGame = Cast<UAuraLoadMenuSaveGame>(SaveGameObject);
	return LoadMenuSaveGame;
}
