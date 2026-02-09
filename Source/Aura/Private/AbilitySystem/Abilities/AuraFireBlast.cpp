// Copyright NattySoft


#include "AbilitySystem/Abilities/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(const int32 Level, const FString& InTitle)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	const FString Title = !InTitle.IsEmpty() ? InTitle : "FIRE BLAST";
	
	return FString::Printf(TEXT(
		"<Title>%s</>"
		"\n\n"
		
		"<Small>Level: </><Level>%d</>"
		"\n"
		"<Small>Mana Cost: </><ManaCost>%.1f</>"
		"\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>"
		"\n\n"
		
		"<Default>Launches %d </>"
		"<Default>fire balls in all directions, each coming back and </>"
		"<Default>exploding upon return, causing </>"
		"<Damage>%d</><Default> radial fire damage with "
		"a chance to burn</>"),
		
		*Title,
		Level,
		ManaCost,
		Cooldown,
		NumFireBalls,
		ScaledDamage
		);
}

FString UAuraFireBlast::GetNextLevelDescription(const int32 Level)
{
	return GetDescription(Level, "NEXT LEVEL");
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	return TArray<AAuraFireBall*>();
}
