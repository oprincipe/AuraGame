// Copyright NattySoft


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);

	Sphere->SetCollisionObjectType(ECC_Projectile);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
	
	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	if (LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	}
}

void AAuraProjectile::Destroyed()
{
	// On clients (non-authority), play impact effects if the projectile hasn't hit anything yet
	if (!bHit && !HasAuthority()) OnHit();
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	Super::Destroyed();
}

void AAuraProjectile::OnHit()
{
	// Avoid playing twice the sounds
	if (bHit) return;
	bHit = true;
	
	PlayEffects();
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if (DamageEffectParams.SourceAbilitySystemComponent)
	{
		// Ignoring self
		const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
		if (SourceAvatarActor == OtherActor) return;
	
		// Ignore friends
		if (!UAuraAbilitySystemLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;
	
		// Hit logic and Play impact effects (sound and visual) when projectile overlaps with something
		if (!bHit) OnHit();
	}
	
	// On server (authority), destroy the projectile
	if (HasAuthority())
	{
		// Get the Ability System Component from the actor hit by the projectile
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			
			const bool bKnockBack = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockBack)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}
			
			// Here's where we know the target
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			
			// Apply the damage gameplay effect to the hit actor's ASC using the effect spec handle stored during projectile spawn
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	// On clients (non-authority), mark the projectile as hit
	// This prevents playing effects twice when the projectile is destroyed
	else
	{
		bHit = true;
	}
}

void AAuraProjectile::PlayEffects_Implementation() const
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}
}
