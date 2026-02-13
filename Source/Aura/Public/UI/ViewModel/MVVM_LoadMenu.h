// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadMenu.generated.h"

class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadMenu : public UMVVMViewModelBase
{
	GENERATED_BODY()
	

public:
	void InitializeLoadSlots();
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotModelClass;
	
	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotViewModelByIndex(const int32 Index);
	
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(const int32 SlotIndex, const FString& EnteredName);
	
	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(const int32 SlotIndex);
	
	UFUNCTION(BlueprintCallable)
	void SelectSlotButtonPressed(const int32 SlotIndex);
	
	
private:
	UPROPERTY() TMap<int32, UMVVM_LoadSlot*> LoadSlots;
	UPROPERTY() TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	UPROPERTY() TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	UPROPERTY() TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
};
