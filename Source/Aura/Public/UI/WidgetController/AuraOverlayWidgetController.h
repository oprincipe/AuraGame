// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraOverlayWidgetController.generated.h"

class UAuraAbilitySystemComponent;
class UAuraAbilityInfo;
class UAuraUserWidget;
struct FGameplayTag;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayTag MessageTag = FGameplayTag();
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText Message = FText();
	UPROPERTY(EditAnywhere, BlueprintReadOnly) TSubclassOf<UAuraUserWidget> MessageWidget;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) UTexture2D* Image = nullptr;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangedSignature, int32, NewStat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAbilityInfo&, Info);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UAuraOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbackToDependencies() override;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FOnMessageWidgetRowSignature OnMessageWidgetRow;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="GAS|Stats")
	FOnPlayerStatChangedSignature OnPlayerLevelChangedDelegate;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget|Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widget|Data")
	TObjectPtr<UAuraAbilityInfo> AbilityInfo;
	
	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
	
	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* InAuraABS);
	
	void OnXPChanged(const int32 NewXP) const;
	
};

template <typename T>
T* UAuraOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	if (DataTable == nullptr) return nullptr;
	return DataTable->FindRow<T>(Tag.GetTagName(), "", false);
}
