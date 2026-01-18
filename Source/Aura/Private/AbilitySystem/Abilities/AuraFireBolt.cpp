// Copyright NattySoft


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AuraGameplayTags.h"

FString UAuraFireBolt::GetDescription(const int32 Level)
{
	const int32 Damage = GetDamageByDamageType(FAuraGameplayTags::Get().Damage_Fire, Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>"
			"\n\n"
			
			"<Small>Level: </><Level>%d</>"
			"\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n"
			
			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"
			"<Damage>%d</><Default> fire damage with "
			"a chance to burn</>"),
			
			Level,
			ManaCost,
			Cooldown,
			Damage
			);
	}

	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL:</>"
			"\n\n"
			
			"<Small>Level: </><Level>%d</>"
			"\n"
			"<Small>Mana Cost: </><ManaCost>%.1f</>"
			"\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>"
			"\n\n"
			
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing: </>"
			"<Damage>%d</><Default> fire damage with "
			"a chance to burn</>"),
			
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			Damage
			);
}

FString UAuraFireBolt::GetNextLevelDescription(const int32 Level)
{
	return GetDescription(Level + 1);
}
