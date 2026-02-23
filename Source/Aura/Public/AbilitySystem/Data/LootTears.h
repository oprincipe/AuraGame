// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTears.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTears|Spawning")
	TSubclassOf<AActor> LootClass;
	
	UPROPERTY(EditAnywhere, Category = "LootTears|Spawning")
	float ChanceToSpawn = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "LootTears|Spawning")
	int32 MaxNumberToSpawn = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "LootTears|Spawning")
	bool bLootLevelOverride = true; // if it's set to true, it will be spawned at the level of the enemy spawning
	
};


/**
 * 
 */
UCLASS()
class AURA_API ULootTears : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();
	
	UPROPERTY(EditDefaultsOnly, Category = "LootTears|Spawning")
	TArray<FLootItem> LootTears;
};
