// Copyright NattySoft


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraAttributeMenuWidgetController.h"
#include "UI/WidgetController/AuraOverlayWidgetController.h"
#include "UI/WidgetController/AuraSpellMenuWidgetController.h"


UAuraOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController) return OverlayWidgetController;
	OverlayWidgetController = NewObject<UAuraOverlayWidgetController>(this, OverlayWidgetControllerClass);
	OverlayWidgetController->SetWidgetControllerParams(Params);
	OverlayWidgetController->BindCallbackToDependencies();
		
	return OverlayWidgetController;
}

UAuraAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (AttributeMenuWidgetController) return AttributeMenuWidgetController;
	AttributeMenuWidgetController = NewObject<UAuraAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
	AttributeMenuWidgetController->SetWidgetControllerParams(Params);
	AttributeMenuWidgetController->BindCallbackToDependencies();
		
	return AttributeMenuWidgetController;
}

UAuraSpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (SpellMenuWidgetController) return SpellMenuWidgetController;
	SpellMenuWidgetController = NewObject<UAuraSpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
	SpellMenuWidgetController->SetWidgetControllerParams(Params);
	SpellMenuWidgetController->BindCallbackToDependencies();
		
	return SpellMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass must be set in the Blueprint"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass must be set in the Blueprint"));
	
	// Create widget with correct type and store reference
	OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
	if (OverlayWidget)
	{
		const FWidgetControllerParams Params = FWidgetControllerParams(PC, PS, ASC, AS);
		UAuraOverlayWidgetController* WidgetController = GetOverlayWidgetController(Params);
		
		OverlayWidget->SetWidgetController(WidgetController);
		WidgetController->BroadcastInitialValues();
		OverlayWidget->AddToViewport();
	}
}
