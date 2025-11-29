#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

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
	
	void SetIsCriticalHit(const bool bInCriticalHit) { bIsCriticalHit = bInCriticalHit; }
	void SetIsBlockHit(const bool bInBlockHit) { bIsBlockHit = bInBlockHit; }
	
protected:

	UPROPERTY() bool bIsBlockHit = false;
	UPROPERTY() bool bIsCriticalHit = false;
	
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
