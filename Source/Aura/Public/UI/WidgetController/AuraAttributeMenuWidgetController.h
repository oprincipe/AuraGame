// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "AuraAttributeMenuWidgetController.generated.h"


class UAttributeInfo;
struct FAuraAttributeInfo;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Attribute);


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

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;
	
};
