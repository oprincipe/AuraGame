// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraBeamSpell.h"
#include "AuraElectrocute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(const int32 Level, const FString& InTitle = "") override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
};
