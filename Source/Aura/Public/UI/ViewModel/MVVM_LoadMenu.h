// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()
	

public:
	/** Delegates */
	UPROPERTY(BlueprintAssignable) FSlotSelected SlotSelected;
	
	/** Properties */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotModelClass;
	
	/** Functions */
	void InitializeLoadSlots();
	
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(const int32 Index);
	
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(const int32 SlotIndex, const FString& EnteredName);
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(const int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(const int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed() const;
	
	void LoadData();
	
private:
	UPROPERTY() TMap<int32, UMVVM_LoadSlot*> LoadSlots;
	UPROPERTY() TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	UPROPERTY() TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	UPROPERTY() TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
	UPROPERTY() UMVVM_LoadSlot* SelectedSlot;
};
