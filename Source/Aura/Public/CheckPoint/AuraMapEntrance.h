// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraCheckPoint.h"
#include "AuraMapEntrance.generated.h"

UCLASS()
class AURA_API AAuraMapEntrance : public AAuraCheckPoint
{
	GENERATED_BODY()

public:
	AAuraMapEntrance(const FObjectInitializer& ObjectInitializer);
	
protected:
	/** Start HighlightInterface */
	virtual void HighlightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	/** End HighlightInterface */
	
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	virtual void LoadActor_Implementation() override;
	
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> DestinationMap;
	
	UPROPERTY(EditAnywhere)
	FName DestinationPlayerStartTag;
};
