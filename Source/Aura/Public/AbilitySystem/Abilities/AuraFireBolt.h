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
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectiles(
		const FVector& ProjectileTargetLocation, 
		const FGameplayTag& SocketTag, 
		const bool bOverridePitch = false, 
		const float PitchOverride = 0.f,
		AActor* HomingTarget = nullptr
		);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float ProjectileSpread = 90.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	int32 MaxNumProjectiles = 5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	bool bLaunchHomingProjectiles = true;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float HomingAccelerationMin = 1600.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float HomingAccelerationMax = 3200.f;
};
