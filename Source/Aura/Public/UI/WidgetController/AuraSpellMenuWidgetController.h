// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTags.h"
#include "AuraWidgetController.h"
#include "AuraSpellMenuWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSpellGlobeSelectedSignature, bool, bSpendPointsButtonEnabled, bool, bEquipButtonEnabled, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);

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
	
	UFUNCTION(BlueprintCallable, Category="GAS|Globe")
	void EquipButtonPressed();
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnPlayerStatChangedSignature SpellPointsDelegate;
	
	/** Delegates */
	UPROPERTY(BlueprintAssignable, Category="GAS|Globe")
	FOnSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Globe")
	FOnWaitForEquipSelectionSignature WaitForEquipDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Globe")
	FOnWaitForEquipSelectionSignature StopWaitingForEquipDelegate;
	
private:
	static void ShouldEnableButtons(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& bShouldEnableSpendPointsButton, bool& bShouldEnableEquipButton);
	
	FSelectedAbility SelectedAbility = { FAuraGameplayTags::Get().Abilities_None, FAuraGameplayTags::Get().Abilities_Status_Locked };
	int32 CurrentSpellPoints = 0;
	
	bool bWaitingForEquipSelection = false;
};
