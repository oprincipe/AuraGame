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
	
	// Start IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface

protected:
#pragma region  AbilitySystem
	UPROPERTY() TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY() TObjectPtr<UAttributeSet> AttributeSet;
#pragma endregion

public:
#pragma region  AbilitySystem
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
#pragma endregion
};
