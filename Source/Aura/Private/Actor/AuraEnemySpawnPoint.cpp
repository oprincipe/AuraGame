// Copyright NattySoft


#include "Actor/AuraEnemySpawnPoint.h"

#include "Character/AuraEnemy.h"

AAuraEnemySpawnPoint::AAuraEnemySpawnPoint()
{
	PrimaryActorTick.bCanEverTick = false;
}


void AAuraEnemySpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform());
	Enemy->SetEnemyLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
