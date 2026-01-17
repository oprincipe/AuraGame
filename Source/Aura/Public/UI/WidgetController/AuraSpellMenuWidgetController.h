// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AuraSpellMenuWidgetController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraSpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnPlayerStatChangedSignature SpellPointsDelegate;
};
