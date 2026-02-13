// Copyright NattySoft


#include "UI/ViewModel/MVVM_LoadMenu.h"

#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadMenu::InitializeLoadSlots()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, LoadSlotModelClass);
	LoadSlots.Add(0, LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, LoadSlotModelClass);
	LoadSlots.Add(1, LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, LoadSlotModelClass);
	LoadSlots.Add(2, LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadMenu::GetLoadSlotViewModelByIndex(const int32 Index)
{
	return LoadSlots[Index];
}

void UMVVM_LoadMenu::NewSlotButtonPressed(const int32 Slot, const FString& EnteredName)
{
}

void UMVVM_LoadMenu::NewGameButtonPressed(const int32 SlotIndex)
{
	LoadSlots[SlotIndex]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadMenu::SelectSlotButtonPressed(const int32 SlotIndex)
{
}
