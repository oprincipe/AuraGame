// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag;
	
	virtual FString GetDescription(const int32 Level);
	virtual FString GetNextLevelDescription(const int32 Level);
	static FString GetLockedDescription(const int32 Level);

protected:
	float GetManaCost(const float InLevel = 1.f) const;
	float GetCooldown(const float InLevel = 1.f) const;
	
};
