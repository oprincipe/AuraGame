// Copyright NattySoft


#include "CheckPoint/AuraMapEntrance.h"

#include "Components/SphereComponent.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AAuraMapEntrance::AAuraMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComponent);
}

void AAuraMapEntrance::HighlightActor_Implementation()
{
	CheckPointMesh->SetCustomDepthStencilValue(CustomDepthStencilOverride);
	CheckPointMesh->SetRenderCustomDepth(true);
	CheckPointMesh->MarkRenderStateDirty();
}

void AAuraMapEntrance::SetMoveToLocation_Implementation(FVector& OutDestination)
{
	Super::SetMoveToLocation_Implementation(OutDestination);
}

void AAuraMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;
	
	bReached = true;
	
	if (const AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		AuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
	}
	
	IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);
	
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
}

void AAuraMapEntrance::LoadActor_Implementation()
{
	// Nothing here
}
