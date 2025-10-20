// Copyright NattySoft


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	// Null checks for safety
	if (GetWorld() && OverlayWidgetClass)
	{
		// Create widget with correct type and store reference
		OverlayWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayWidgetClass);
		if (OverlayWidget)
		{
			OverlayWidget->AddToViewport();
		}
	}
}
