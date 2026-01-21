#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"


class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() = default;
	
	UPROPERTY() TObjectPtr<UObject> WorldContext = nullptr;
	UPROPERTY() TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	UPROPERTY() TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	UPROPERTY() TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	
	UPROPERTY() float BaseDamage = 0.f;
	UPROPERTY() float AbilityLevel = 1.f;
	UPROPERTY() FGameplayTag DamageType = FGameplayTag();
	UPROPERTY() float DebuffChance = 0.f;
	UPROPERTY() float DebuffDamage = 0.f;
	UPROPERTY() float DebuffDuration = 0.f;
	UPROPERTY() float DebuffFrequency = 0.f;
	
};




USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
	
public:
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	
	/** 
	 * Creates a copy of this context, used to duplicate for later modifications 
	 * @see GameplayEffectTypes.h
	 */
	virtual FAuraGameplayEffectContext* Duplicate() const override
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	
	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsBlockHit() const { return bIsBlockHit; }
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff; }
	float GetDebuffDamage() const { return DebuffDamage; }
	float GetDebuffDuration() const { return DebuffDuration; }
	float GetDebuffFrequency() const { return DebuffFrequency; }
	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	
	void SetIsCriticalHit(const bool bInCriticalHit) { bIsCriticalHit = bInCriticalHit; }
	void SetIsBlockHit(const bool bInBlockHit) { bIsBlockHit = bInBlockHit; }
	void SetIsSuccessfulDebuff(const bool bIsSuccessful) { bIsSuccessfulDebuff = bIsSuccessful; }
	void SetDebuffDamage(const float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(const float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(const float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }

	
protected:

	UPROPERTY() bool bIsBlockHit = false;
	UPROPERTY() bool bIsCriticalHit = false;
	UPROPERTY() bool bIsSuccessfulDebuff = false;
	UPROPERTY() float DebuffDamage = 0.f;
	UPROPERTY() float DebuffDuration = 0.f;
	UPROPERTY() float DebuffFrequency = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;
};

/**
 * @see GameplayEffectTypes.h
 */
template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};
