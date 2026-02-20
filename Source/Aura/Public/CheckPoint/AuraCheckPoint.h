// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "Aura/Aura.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "AuraCheckPoint.generated.h"

class USphereComponent;

UCLASS()
class AURA_API AAuraCheckPoint : public APlayerStart, public ISaveInterface, public IHighlightInterface
{
	GENERATED_BODY()

public:
	AAuraCheckPoint(const FObjectInitializer& ObjectInitializer);

	/** Start HighlightInterface */
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	/** End HighlightInterface */
	
	/** Start SaveInterface */
	virtual bool ShouldLoadTransform_Implementation() const override;
	virtual void LoadActor_Implementation() override;
	/** End SaveInterface */
	
	UPROPERTY(BlueprintReadOnly, SaveGame) 
	bool bReached = false;
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION() virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	void HandleGlowEffects();
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;
	
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess=true))
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;
};
