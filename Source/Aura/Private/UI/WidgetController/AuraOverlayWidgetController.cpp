// Copyright NattySoft


#include "UI/WidgetController/AuraOverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AuraAbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/AuraPlayerState.h"

void UAuraOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UAuraOverlayWidgetController::BindCallbackToDependencies()
{
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnXPChangeDelegate.AddUObject(this, &UAuraOverlayWidgetController::OnXPChanged);
	
	
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).
		AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});
	
	if (UAuraAbilitySystemComponent* AuraABS = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraABS->bStartupAbilitiesGiven)
		{
			// In case the ability has been already given, we don't need the delegate binding
			OnInitializeStartupAbilities(AuraABS);
		}
		else
		{
			// Bind the delegate in case the ability has not been given yet
			AuraABS->AbilitiesGivenDelegate.AddUObject(this, &UAuraOverlayWidgetController::OnInitializeStartupAbilities);
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

void UAuraOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* InAuraABS)
{
	if (!InAuraABS->bStartupAbilitiesGiven) return;
	
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, InAuraABS](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(InAuraABS->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = InAuraABS->GetInputTagFromSpec(AbilitySpec);
		
		AbilityInfoDelegate.Broadcast(Info);
	});
	
	InAuraABS->ForEachAbility(BroadcastDelegate);
}

void UAuraOverlayWidgetController::OnXPChanged(const int32 NewXP) const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
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
