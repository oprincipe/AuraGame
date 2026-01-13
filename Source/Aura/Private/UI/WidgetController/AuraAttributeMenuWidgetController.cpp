// Copyright NattySoft


#include "UI/WidgetController/AuraAttributeMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "Player/AuraPlayerState.h"

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
	
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnAttributePointsChangeDelegate.AddLambda([this](int32 Points)
	{
		OnAttributePointsChangedDelegate.Broadcast(Points);
	});
}

void UAuraAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);

	checkf(AttributeInfo, TEXT("AttributeInfo is not set"));

	for (auto& Pair : AS->TagsToAttributesMap)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value);
	}

	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	OnAttributePointsChangedDelegate.Broadcast(AuraPlayerState->GetAttributePoints());
}

void UAuraAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttribute(AttributeTag);
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
