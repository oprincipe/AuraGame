// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "AuraCheckPoint.generated.h"

class USphereComponent;

UCLASS()
class AURA_API AAuraCheckPoint : public APlayerStart
{
	GENERATED_BODY()

public:
	AAuraCheckPoint(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	UFUNCTION() virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	void HandleGlowEffects();
	
private:
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
	
};
