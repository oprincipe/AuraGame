// Copyright NattySoft


#include "UI/WidgetController/AuraAttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

void UAuraAttributeMenuWidgetController::BindCallbackToDependencies()
{
	check(AttributeInfo);
	for (auto& Pair : GetAuraAttributeSet()->TagsToAttributesMap)
	{
		GetAuraAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value);
			}
		);	
	}
	
	GetAuraPlayerState()->OnAttributePointsChangeDelegate.AddLambda([this](int32 Points)
	{
		OnAttributePointsChangedDelegate.Broadcast(Points);
	});
}

void UAuraAttributeMenuWidgetController::BroadcastInitialValues()
{
	for (auto& Pair : GetAuraAttributeSet()->TagsToAttributesMap)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}

	OnAttributePointsChangedDelegate.Broadcast(GetAuraPlayerState()->GetAttributePoints());
}

void UAuraAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraAbilitySystemComponent()->UpgradeAttribute(AttributeTag);
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
