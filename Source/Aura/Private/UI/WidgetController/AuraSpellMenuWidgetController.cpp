// Copyright NattySoft


#include "UI/WidgetController/AuraSpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraSpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
}

void UAuraSpellMenuWidgetController::BindCallbackToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
}
