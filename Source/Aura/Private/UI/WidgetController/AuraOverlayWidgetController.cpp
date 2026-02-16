// Copyright NattySoft


#include "UI/WidgetController/AuraOverlayWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UAuraOverlayWidgetController::BroadcastInitialValues()
{
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
}

void UAuraOverlayWidgetController::BindCallbackToDependencies()
{
	GetAuraPlayerState()->OnXPChangeDelegate.AddUObject(this, &UAuraOverlayWidgetController::OnXPChanged);
	GetAuraPlayerState()->OnLevelChangeDelegate.AddLambda([this](const int32 NewLevel, const bool bLevelUp)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel, bLevelUp);
	});
	
	if (UAuraAbilitySystemComponent* AuraABS = GetAuraAbilitySystemComponent())
	{
		AuraABS->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});

		AuraABS->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute()).
			AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});

		AuraABS->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute()).
			AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnManaChanged.Broadcast(Data.NewValue);
			});
	
		AuraABS->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute()).
			AddLambda([this](const FOnAttributeChangeData& Data)
			{
				OnMaxManaChanged.Broadcast(Data.NewValue);
			});
		
		// Binding Equipped button pressed on the spell menu
		AuraABS->AbilityEquippedDelegate.AddUObject(this, &UAuraOverlayWidgetController::OnAbilityEquipped);
		
		if (AuraABS->bStartupAbilitiesGiven)
		{
			// In case the ability has been already given, we don't need the delegate binding
			BroadcastAbilityInfo();
		}
		else
		{
			// Bind the delegate in case the ability has not been given yet
			AuraABS->AbilitiesGivenDelegate.AddUObject(this, &UAuraOverlayWidgetController::BroadcastAbilityInfo);
		}
		
		AuraABS->EffectAssetTagsDelegate.
			AddLambda([this](const FGameplayTagContainer& GameplayTags)
			{
				for (const FGameplayTag& Tag : GameplayTags)
				{
					// Load a root tag for messages
					const FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (!Tag.MatchesTag(MessageTag)) continue;

					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					if (Row) OnMessageWidgetRow.Broadcast(*Row);
				}
			});
	}
}

void UAuraOverlayWidgetController::OnXPChanged(const int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetAuraPlayerState()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("AuraPlayerState has no LevelUpInfo assigned"));
	
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();
	
	// Delta of xp to the end of the level
	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
		
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UAuraOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PreviousSlot) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	
	FAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Abilities_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Abilities_None;
	
	// Broadcast empty info if the previous slot is a valid slot
	// only if equipping an equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	
	// Broadcast the current slot
	FAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
