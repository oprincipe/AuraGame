// Copyright NattySoft


#include "UI/HUD/AuraLoadMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/ViewModel/MVVM_LoadMenu.h"
#include "UI/Widget/LoadMenuWidget.h"

void AAuraLoadMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	LoadMenuViewModel = NewObject<UMVVM_LoadMenu>(this, LoadMenuViewModelClass);
	LoadMenuViewModel->InitializeLoadSlots();
	
	LoadMenuWidget = CreateWidget<ULoadMenuWidget>(GetWorld(), LoadMenuWidgetClass);
	LoadMenuWidget->AddToViewport();
	LoadMenuWidget->BlueprintInitializeWidget();
	
	LoadMenuViewModel->LoadData();
}
