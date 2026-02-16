// Copyright NattySoft


#include "UI/ViewModel/MVVM_LoadMenu.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlot_0->SlotIndex = 0;
	LoadSlots.Add(0, LoadSlot_0);
	
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlot_1->SlotIndex = 1;
	LoadSlots.Add(1, LoadSlot_1);
	
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotModelClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlot_2->SlotIndex = 2;
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadMenu::GetLoadSlotViewModelByIndex(const int32 Index)
{
	return LoadSlots[Index];
}

void UMVVM_LoadMenu::NewSlotButtonPressed(const int32 SlotIndex, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!AuraGameMode) return;
	
	LoadSlots[SlotIndex]->SetMapName(AuraGameMode->DefaultMapName);
	LoadSlots[SlotIndex]->SetPlayerName(EnteredName);
	LoadSlots[SlotIndex]->SlotStatus = Taken;
	LoadSlots[SlotIndex]->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	
	AuraGameMode->SaveSlotData(LoadSlots[SlotIndex], SlotIndex);
	LoadSlots[SlotIndex]->InitializeSlot();
	
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (!AuraGameInstance) return;
	AuraGameInstance->LoadSlotName = LoadSlots[SlotIndex]->LoadSlotName;
	AuraGameInstance->LoadSlotIndex = SlotIndex;
	AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
	
}

void UMVVM_LoadMenu::NewGameButtonPressed(const int32 SlotIndex)
{
	LoadSlots[SlotIndex]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadMenu::SelectSlotButtonPressed(const int32 SlotIndex)
{
	SlotSelected.Broadcast();
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		LoadSlot.Value->EnableSelectSlotButton.Broadcast(LoadSlot.Key != SlotIndex);
	}
	SelectedSlot = LoadSlots[SlotIndex];
}

void UMVVM_LoadMenu::DeleteButtonPressed() const
{
	if (!IsValid(SelectedSlot)) return;
	AAuraGameModeBase::DeleteSlotData(SelectedSlot->LoadSlotName, SelectedSlot->SlotIndex);
	SelectedSlot->SlotStatus = Vacant;
	SelectedSlot->InitializeSlot();
	SelectedSlot->EnableSelectSlotButton.Broadcast(true);
}

void UMVVM_LoadMenu::PlayButtonPressed() const
{
	if (!SelectedSlot) return;
	
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!AuraGameMode) return;
	
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance());
	if (!AuraGameInstance) return;
	
	AuraGameInstance->PlayerStartTag = SelectedSlot->PlayerStartTag;
	AuraGameMode->TravelToMap(SelectedSlot);
}

void UMVVM_LoadMenu::LoadData()
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!AuraGameMode) return;
	
	for (const TTuple<int32, UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		const UAuraLoadMenuSaveGame* SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->LoadSlotName, LoadSlot.Key);
		
		const FString PlayerName = SaveObject->PlayerName;
		const FString MapName = SaveObject->MapName;
		const TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = SaveObject->SaveSlotStatus;
		
		LoadSlot.Value->SlotStatus = SaveSlotStatus;
		LoadSlot.Value->PlayerStartTag = SaveObject->PlayerStartTag;
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SetMapName(MapName);
		LoadSlot.Value->InitializeSlot();
	}
}
