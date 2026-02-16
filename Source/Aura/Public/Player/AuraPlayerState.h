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
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /** Stat value */, bool /** bLevelUp */);

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
	FOnAuraPlayerStatChanged OnAttributePointsChangeDelegate;
	FOnAuraPlayerStatChanged OnSpellPointsChangeDelegate;
	FOnLevelChanged OnLevelChangeDelegate;
	
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
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	
	void AddToXP(const int32 InXP);
	void AddToLevel(const int32 InLevel);
	void AddToAttributePoints(const int32 InAttributePoints);
	void AddToSpellPoints(const int32 InSpellPoints);
	
	void SetXP(const int32 InXP);
	void SetLevel(const int32 InLevel);
	void SetAttributePoints(const int32 InAttributePoints);
	void SetSpellPoints(const int32 InSpellPoints);
	
	
	UPROPERTY(EditDefaultsOnly) TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level) 
	int32 Level = 1;
	UFUNCTION() 
	void OnRep_Level(const int32& OldLevel) const;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP) 
	int32 XP = 0;
	UFUNCTION() 
	void OnRep_XP(const int32 OldXP) const;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints) 
	int32 AttributePoints = 0;
	UFUNCTION() 
	void OnRep_AttributePoints(const int32 OldAttributePoints) const;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints) 
	int32 SpellPoints = 0;
	UFUNCTION() 
	void OnRep_SpellPoints(const int32 OldSpellPoints) const;
};
