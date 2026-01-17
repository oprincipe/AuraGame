// Copyright NattySoft


#include "UI/WidgetController/AuraSpellMenuWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"

void UAuraSpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoints());
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
	
	GetAuraPlayerState()->OnSpellPointsChangeDelegate.AddLambda([this](const int32 Points)
	{
		SpellPointsDelegate.Broadcast(Points);
	});
}
