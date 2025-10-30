// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AuraAttributeMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbackToDependencies() override;
	virtual void BroadcastInitialValues() override;
};
