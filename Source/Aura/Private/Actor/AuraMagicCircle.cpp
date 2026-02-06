// Copyright NattySoft


#include "Actor/AuraMagicCircle.h"

#include "Components/DecalComponent.h"

AAuraMagicCircle::AAuraMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);
	
	MagicCircleDecal = CreateDefaultSubobject<UDecalComponent>("MagicCircleDecal");
	MagicCircleDecal->SetupAttachment(GetRootComponent());
}

void AAuraMagicCircle::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

