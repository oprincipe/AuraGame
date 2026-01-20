// Copyright NattySoft


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannel.h"

FAbilityInfo UAuraAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, const bool bLogNotFound)
{
	for (const FAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Can't find info for AbilityTag [%s] on AbilityInfo [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	
	return FAbilityInfo();
}
