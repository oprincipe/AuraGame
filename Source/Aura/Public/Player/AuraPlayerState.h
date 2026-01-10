// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class ULevelUpInfo;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAuraPlayerStatChanged, int32 /** Stat value */);

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

	FOnAuraPlayerStatChanged OnXPChangeDelegate;
	FOnAuraPlayerStatChanged OnLevelChangeDelegate;

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
	FORCEINLINE int32 GetXP() const { return XP; }
	
	void AddToXP(const int32 InXP);
	void AddToLevel(const int32 InLevel);
	
	void SetXP(const int32 InXP);
	void SetLevel(const int32 InLevel);
	
	UPROPERTY(EditDefaultsOnly) TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level) int32 Level = 1;
	UFUNCTION() void OnRep_Level(const int32& OldLevel) const;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level) int32 XP = 1;
	UFUNCTION() void OnRep_XP(const int32 OldXP) const;
};
