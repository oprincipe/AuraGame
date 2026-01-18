// Copyright NattySoft


#include "UI/WidgetController/AuraSpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"

void UAuraSpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsDelegate.Broadcast(GetAuraPlayerState()->GetSpellPoints());
}

void UAuraSpellMenuWidgetController::BindCallbackToDependencies()
{
	GetAuraAbilitySystemComponent()->AbilityStatusChangedDelegate.AddLambda([this](
		const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpellPointsButton = false;
			bool bEnableEquipButton = false;
			ShouldEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpellPointsButton, bEnableEquipButton);
			
			FString AbilityDescription;
			FString AbilityNextLevelDescription;
			GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, AbilityDescription, AbilityNextLevelDescription);
			
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPointsButton, bEnableEquipButton, AbilityDescription, AbilityNextLevelDescription);
		}
		
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
		CurrentSpellPoints = Points;
		
		bool bEnableSpellPointsButton = false;
		bool bEnableEquipButton = false;
		ShouldEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpellPointsButton, bEnableEquipButton);
		
		FString AbilityDescription;
		FString AbilityNextLevelDescription;
		GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, AbilityDescription, AbilityNextLevelDescription);
	
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPointsButton, bEnableEquipButton, AbilityDescription, AbilityNextLevelDescription);
	});
}

void UAuraSpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTags.Abilities_None);
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag); 
	const bool bSpecValid = AbilitySpec != nullptr;
	
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusFromSpec(*AbilitySpec);
	}
	
	// Update the local struct to track the changes so it can be used on BindCallbacksToDependencies
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	
	bool bEnableSpellPointsButton = false;
	bool bEnableEquipButton = false;
	ShouldEnableButtons(AbilityStatus, SpellPoints, bEnableSpellPointsButton, bEnableEquipButton);
	
	FString AbilityDescription;
	FString AbilityNextLevelDescription;
	GetAuraAbilitySystemComponent()->GetDescriptionsByAbilityTag(AbilityTag, AbilityDescription, AbilityNextLevelDescription);
	
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPointsButton, bEnableEquipButton, AbilityDescription, AbilityNextLevelDescription);
}

void UAuraSpellMenuWidgetController::SpendPointButtonPressed()
{
	if (!GetAuraAbilitySystemComponent()) return;
	GetAuraAbilitySystemComponent()->Server_SpendSpellPoints(SelectedAbility.Ability);
}

void UAuraSpellMenuWidgetController::ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints,
                                                         bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	
	bShouldEnableEquipButton = false;
	bShouldEnableSpendPointsButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpendPointsButton = true;
		}
	}
}
