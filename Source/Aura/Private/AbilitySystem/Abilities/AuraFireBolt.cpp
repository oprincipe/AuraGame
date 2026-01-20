// Copyright NattySoft


#include "AbilitySystem/Abilities/AuraFireBolt.h"

FString UAuraFireBolt::GetDescription(const int32 Level, const FString& InTitle)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	const FString Title = !InTitle.IsEmpty() ? InTitle : "FIRE BOLT";
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>%s</>"
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
			
			*Title,
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage
			);
	}

	return FString::Printf(TEXT(
			"<Title>%s:</>"
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
			
			*Title,
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage
			);
}

FString UAuraFireBolt::GetNextLevelDescription(const int32 Level)
{
	return GetDescription(Level + 1, "NEXT LEVEL");
}
