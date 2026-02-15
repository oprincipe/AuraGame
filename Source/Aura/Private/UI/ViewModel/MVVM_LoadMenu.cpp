// Copyright NattySoft


#include "UI/ViewModel/MVVM_LoadMenu.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotModelClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotModelClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotModelClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadMenu::GetLoadSlotViewModelByIndex(const int32 Index)
{
	return LoadSlots[Index];
}

void UMVVM_LoadMenu::NewSlotButtonPressed(const int32 SlotIndex, const FString& EnteredName)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	LoadSlots[SlotIndex]->PlayerName = EnteredName;
	AuraGameMode->SaveSlotData(LoadSlots[SlotIndex], SlotIndex);
	LoadSlots[SlotIndex]->InitializeSlot();
}

void UMVVM_LoadMenu::NewGameButtonPressed(const int32 SlotIndex)
{
	LoadSlots[SlotIndex]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadMenu::SelectSlotButtonPressed(const int32 SlotIndex)
{
}
