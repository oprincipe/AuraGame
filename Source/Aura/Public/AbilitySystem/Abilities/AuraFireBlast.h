// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(const int32 Level, const FString& InTitle = "") override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
	
	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fire Blast")
	int32 NumFireBalls = 12;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraFireBall> FireBallClass;
};
