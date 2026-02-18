// Copyright NattySoft


#include "Game/AuraLoadMenuSaveGame.h"

FSavedMap UAuraLoadMenuSaveGame::GetSavedMapWithMapName(const FString& InMapName)
{
	for (const FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapAssetName == InMapName)
		{
			return SavedMap;
		}
	}
	return FSavedMap();
}

bool UAuraLoadMenuSaveGame::HasMap(const FString& InMapName)
{
	for (const FSavedMap& SavedMap : SavedMaps)
	{
		if (SavedMap.MapAssetName == InMapName)
		{
			return true;
		}
	}
	return false;
}
