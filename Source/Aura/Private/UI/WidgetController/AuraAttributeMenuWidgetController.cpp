// Copyright NattySoft


#include "UI/WidgetController/AuraAttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAuraAttributeMenuWidgetController::BindCallbackToDependencies()
{
	
}

void UAuraAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("AttributeInfo is not set"));

	for (auto& Pair : AS->TagsToAttributesMap)
	{
		FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value.GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);		
	}
}
