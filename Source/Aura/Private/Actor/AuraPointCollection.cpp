// Copyright NattySoft


#include "Actor/AuraPointCollection.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


AAuraPointCollection::AAuraPointCollection()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	Pt_1 = CreateDefaultSubobject<USceneComponent>("Pt_1");
	Pt_2 = CreateDefaultSubobject<USceneComponent>("Pt_2");
	Pt_3 = CreateDefaultSubobject<USceneComponent>("Pt_3");
	Pt_4 = CreateDefaultSubobject<USceneComponent>("Pt_4");
	Pt_5 = CreateDefaultSubobject<USceneComponent>("Pt_5");
	Pt_6 = CreateDefaultSubobject<USceneComponent>("Pt_6");
	Pt_7 = CreateDefaultSubobject<USceneComponent>("Pt_7");
	Pt_8 = CreateDefaultSubobject<USceneComponent>("Pt_8");
	Pt_9 = CreateDefaultSubobject<USceneComponent>("Pt_9");
	Pt_10 = CreateDefaultSubobject<USceneComponent>("Pt_10");
	
	SetRootComponent(Pt_0);
	Pt_1->SetupAttachment(Pt_0);
	Pt_2->SetupAttachment(Pt_0);
	Pt_3->SetupAttachment(Pt_0);
	Pt_4->SetupAttachment(Pt_0);
	Pt_5->SetupAttachment(Pt_0);
	Pt_6->SetupAttachment(Pt_0);
	Pt_7->SetupAttachment(Pt_0);
	Pt_8->SetupAttachment(Pt_0);
	Pt_9->SetupAttachment(Pt_0);
	Pt_10->SetupAttachment(Pt_0);
	
	ImmutablePts.Add(Pt_0);
	ImmutablePts.Add(Pt_1);
	ImmutablePts.Add(Pt_2);
	ImmutablePts.Add(Pt_3);
	ImmutablePts.Add(Pt_4);
	ImmutablePts.Add(Pt_5);
	ImmutablePts.Add(Pt_6);
	ImmutablePts.Add(Pt_7);
	ImmutablePts.Add(Pt_8);
	ImmutablePts.Add(Pt_9);
	ImmutablePts.Add(Pt_10);
	
}

void AAuraPointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<USceneComponent*> AAuraPointCollection::GetGroundPoints(const FVector& GroundLocation, const int32 NumPoints,
	const float YawOverride)
{
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("NumPoints must be less than or equal to the number of immutable points"));
	
	TArray<USceneComponent*> ArrayCopy;
	for (USceneComponent* Pt : ImmutablePts)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;
		
		// If the current point is different from the reference point
		if (Pt != Pt_0)
		{
			// Calculate the vector from the reference point (Pt_0) to the current point (Pt)
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
    
			// Rotate that vector around the vertical axis (yaw) by YawOverride degrees
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
    
			// Set the current point's new world location by adding the rotated vector to the reference point's location
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}
		
		const FVector RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		const FVector LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
		
		// Find all players and enemies that I want to ignore during the point line trace
		FHitResult HitResult;
		TArray<AActor*> IgnoreActors;
		UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, IgnoreActors, TArray<AActor*>(), 1500.f, GetActorLocation());
		
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActors);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);
		
		const FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));
		
		// Add the adjusted point to the array copy
		ArrayCopy.Add(Pt);
	}
	
	return ArrayCopy;
}
