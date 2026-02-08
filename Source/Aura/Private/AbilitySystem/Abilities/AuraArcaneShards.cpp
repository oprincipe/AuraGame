// Copyright NattySoft


#include "AbilitySystem/Abilities/AuraArcaneShards.h"

FString UAuraArcaneShards::GetDescription(const int32 Level, const FString& InTitle)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	const FString Title = !InTitle.IsEmpty() ? InTitle : "ARCANE SHARDS";
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
			
			"<Default>Summon shards of arcane energy, "
			"causing radial arcane damage of: </>"
			"<Damage>%d</><Default> at the shard origin.</>"),
			
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
			
			"<Default>Summon %d shards of arcane energy, "
			"causing radial arcane damage of: </>"
			"<Damage>%d</><Default></>"),
			
			*Title,
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShards - 1),
			ScaledDamage
			);
}

FString UAuraArcaneShards::GetNextLevelDescription(const int32 Level)
{
	return GetDescription(Level + 1, "NEXT LEVEL");
}
