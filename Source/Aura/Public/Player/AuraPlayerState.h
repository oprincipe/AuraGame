// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Start IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface

protected:
#pragma region  AbilitySystemProperties
	UPROPERTY(VisibleAnywhere) TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY() TObjectPtr<UAttributeSet> AttributeSet;
#pragma endregion

public:
#pragma region  AbilitySystemFunctions
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
#pragma endregion

	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level) int32 Level = 1;
	UFUNCTION() void OnRep_Level(const int32& OldLevel) const;
	
};
