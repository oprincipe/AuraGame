// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(const int32 Level, const FString& InTitle = "") override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
};
