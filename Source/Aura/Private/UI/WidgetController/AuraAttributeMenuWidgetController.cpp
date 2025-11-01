// Copyright NattySoft


#include "UI/WidgetController/AuraAttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAuraAttributeMenuWidgetController::BindCallbackToDependencies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	for (auto& Pair : AS->TagsToAttributesMap)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value);
			}
		);	
	}
}

void UAuraAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("AttributeInfo is not set"));

	for (auto& Pair : AS->TagsToAttributesMap)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}
}

void UAuraAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& Tag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	if (const float NewValue = Attribute.GetNumericValue(AttributeSet); Info.AttributeValue != NewValue)
	{
		Info.AttributeValue = NewValue;
		AttributeInfoDelegate.Broadcast(Info);
	}
}
