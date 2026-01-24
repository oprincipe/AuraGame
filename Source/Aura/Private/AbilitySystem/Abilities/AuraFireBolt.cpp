// Copyright NattySoft


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

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

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	const bool bOverridePitch, const float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag
	);
	
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	
	const FVector Forward = Rotation.Vector();

	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	TArray<FRotator> Rotations = UAuraAbilitySystemLibrary::EvenlySpaceRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);
	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation - FVector(0.f, 0.f, 10.f));
		SpawnTransform.SetRotation(Rot.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	
		if (IsValid(HomingTarget) && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			// The Projectile->HomingTargetSceneComponent is necessary for garbage collection
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
	
	
	
}
