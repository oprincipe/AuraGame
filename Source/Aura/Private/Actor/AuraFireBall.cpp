// Copyright NattySoft


#include "Actor/AuraFireBall.h"


AAuraFireBall::AAuraFireBall()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void AAuraFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}
