// Copyright NattySoft


#include "AbilitySystem/Data/LootTears.h"

TArray<FLootItem> ULootTears::GetLootItems()
{
	TArray<FLootItem> ReturnItems;
	
	for (const FLootItem& Item : LootTears)
	{
		for (int32 i = 0; i < Item.MaxNumberToSpawn; i++)
		{
			if (FMath::FRandRange(1.f, 100.f) < Item.ChanceToSpawn)
			{
				FLootItem NewItem;
				NewItem.LootClass = Item.LootClass;
				NewItem.bLootLevelOverride = Item.bLootLevelOverride;
				ReturnItems.Add(NewItem);
			}
		}
	}
	
	return ReturnItems;
}
