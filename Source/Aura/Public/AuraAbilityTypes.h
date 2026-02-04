#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"


class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() = default;
	
	UPROPERTY(BlueprintReadWrite) TObjectPtr<UObject> WorldContext = nullptr;
	UPROPERTY(BlueprintReadWrite) TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	UPROPERTY(BlueprintReadWrite) TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	UPROPERTY(BlueprintReadWrite) TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	
	UPROPERTY(BlueprintReadWrite) float BaseDamage = 0.f;
	UPROPERTY(BlueprintReadWrite) float AbilityLevel = 1.f;
	UPROPERTY(BlueprintReadWrite) FGameplayTag DamageType = FGameplayTag();
	UPROPERTY(BlueprintReadWrite) float DebuffChance = 0.f;
	UPROPERTY(BlueprintReadWrite) float DebuffDamage = 0.f;
	UPROPERTY(BlueprintReadWrite) float DebuffDuration = 0.f;
	UPROPERTY(BlueprintReadWrite) float DebuffFrequency = 0.f;
	UPROPERTY(BlueprintReadWrite) float DeathImpulseMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector DeathImpulse = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite) float KnockbackChance = 0.f;
	UPROPERTY(BlueprintReadWrite) float KnockbackForceMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector KnockbackForce = FVector::ZeroVector;
	
	/** Radial Damage Properties */
	UPROPERTY(BlueprintReadWrite) bool bIsRadialDamage = false;
	UPROPERTY(BlueprintReadWrite) float RadialDamageInnerRadius = 0.f;
	UPROPERTY(BlueprintReadWrite) float RadialDamageOuterRadius = 0.f;
	UPROPERTY(BlueprintReadWrite) FVector RadialDamageOrigin = FVector::ZeroVector;
	
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
	FVector GetDeathImpulse() const { return DeathImpulse; }
	FVector GetKnockbackForce() const { return KnockbackForce; }
	bool IsRadialDamage() const { return bIsRadialDamage; }
	float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }
	
	void SetIsCriticalHit(const bool bInCriticalHit) { bIsCriticalHit = bInCriticalHit; }
	void SetIsBlockHit(const bool bInBlockHit) { bIsBlockHit = bInBlockHit; }
	void SetIsSuccessfulDebuff(const bool bIsSuccessful) { bIsSuccessfulDebuff = bIsSuccessful; }
	void SetDebuffDamage(const float InDamage) { DebuffDamage = InDamage; }
	void SetDebuffDuration(const float InDuration) { DebuffDuration = InDuration; }
	void SetDebuffFrequency(const float InFrequency) { DebuffFrequency = InFrequency; }
	void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }
	void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	void SetKnockbackForce(const FVector& InKnockbackForce) { KnockbackForce = InKnockbackForce; }
	void SetIsRadialDamage(const bool bInIsRadialDamage) { bIsRadialDamage = bInIsRadialDamage; }
	void SetRadialDamageInnerRadius(const float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	void SetRadialDamageOuterRadius(const float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
	
protected:

	UPROPERTY() bool bIsBlockHit = false;
	UPROPERTY() bool bIsCriticalHit = false;
	UPROPERTY() bool bIsSuccessfulDebuff = false;
	UPROPERTY() float DebuffDamage = 0.f;
	UPROPERTY() float DebuffDuration = 0.f;
	UPROPERTY() float DebuffFrequency = 0.f;
	UPROPERTY() FVector KnockbackForce = FVector::ZeroVector;
	
	TSharedPtr<FGameplayTag> DamageType;
	
	UPROPERTY() FVector DeathImpulse = FVector::ZeroVector;
	
	/** Radial Damage Properties */
	UPROPERTY() bool bIsRadialDamage = false;
	UPROPERTY() float RadialDamageInnerRadius = 0.f;
	UPROPERTY() float RadialDamageOuterRadius = 0.f;
	UPROPERTY() FVector RadialDamageOrigin = FVector::ZeroVector;
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
