// Copyright NattySoft


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot()
{
	// todo: check slot status based on loaded data
	SetWidgetSwitcherIndex.Broadcast(2);
}
