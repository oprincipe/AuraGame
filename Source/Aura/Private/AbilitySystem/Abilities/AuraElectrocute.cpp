// Copyright NattySoft


#include "AbilitySystem/Abilities/AuraElectrocute.h"

FString UAuraElectrocute::GetDescription(const int32 Level, const FString& InTitle)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	const FString Title = !InTitle.IsEmpty() ? InTitle : "ELECTROCUTE";
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
			
			"<Default>Emits a beam of lighting, "
			"connecting with the target, repeatedly causing: </>"
			"<Damage>%d</><Default> lightning damage with "
			"a chance to stun</>"),
			
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
			
			"<Default>Emits a beam of lighting, "
			"propagating to %d additional targets nearby, causing: </>"
			"<Damage>%d</><Default> lightning damage with "
			"a chance to stun</>"),
			
			*Title,
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, MaxNumShockTargets - 1),
			ScaledDamage
			);
}

FString UAuraElectrocute::GetNextLevelDescription(const int32 Level)
{
	return GetDescription(Level + 1, "NEXT LEVEL");
}
