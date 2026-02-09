// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

struct FGameplayEffectSpecHandle;
class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();
	UPROPERTY(VisibleAnywhere) TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true)) FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) TObjectPtr<USphereComponent> Sphere;
	
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	void OnHit();
	UFUNCTION() virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	bool bHit = false;
	
	UPROPERTY(EditDefaultsOnly) float LifeSpan = 15.0f;
	UPROPERTY(EditAnywhere) TObjectPtr<UNiagaraSystem> ImpactEffect;
	UPROPERTY(EditAnywhere) TObjectPtr<USoundBase> ImpactSound;
	UPROPERTY(EditAnywhere) TObjectPtr<USoundBase> LoopingSound;
	UPROPERTY() TObjectPtr<UAudioComponent> LoopingSoundComponent;
	
	UFUNCTION(NetMulticast, Reliable)
	void PlayEffects() const;
};
