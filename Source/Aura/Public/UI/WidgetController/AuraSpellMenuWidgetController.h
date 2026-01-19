// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "AuraWidgetController.h"
#include "AuraSpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);

struct FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
};


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraSpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;
	
	UFUNCTION(BlueprintCallable, Category="GAS|Globe")
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable, Category="GAS|Globe")
	void SpendPointButtonPressed();
	
	UFUNCTION(BlueprintCallable, Category="GAS|Globe")
	void GlobeDeselect();
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnPlayerStatChangedSignature SpellPointsDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Globe")
	FOnSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
	
	
private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton);
	
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
};
