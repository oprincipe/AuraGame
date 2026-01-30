// Copyright NattySoft


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/AuraDebuffNiagaraComponent.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	BurnDebuffComponent = CreateDefaultSubobject<UAuraDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Burn;
	
	StunDebuffComponent = CreateDefaultSubobject<UAuraDebuffNiagaraComponent>("StunDebuffComponent");
	StunDebuffComponent->SetupAttachment(GetRootComponent());
	StunDebuffComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Stun;
	
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AAuraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void AAuraCharacterBase::OnRep_Burned()
{
	// Implemented on child classes where's needed
}

void AAuraCharacterBase::OnRep_Stunned()
{
	// Implemented on child classes where's needed
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAuraCharacterBase, bIsBurned);
	DOREPLIFETIME(AAuraCharacterBase, bIsStunned);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 AAuraCharacterBase::GetPlayerLevel_Implementation() const
{
	return 1;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon) && Weapon->GetSkeletalMeshAsset())
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}

	const USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (!IsValid(CharacterMesh)) return FVector::ZeroVector;
	
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Left_Hand))
	{
		return CharacterMesh->GetSocketLocation(LeftHandSocketName);
	}
	
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Right_Hand))
	{
		return CharacterMesh->GetSocketLocation(RightHandSocketName);
	}
	
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return CharacterMesh->GetSocketLocation(TailSocketName);
	}
	
	return CharacterMesh->GetSocketLocation(WeaponTipSocketName);
}

FName AAuraCharacterBase::GetWeaponSocketName_Implementation()
{
	return WeaponTipSocketName;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation() const
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die(const FVector& DeathImpulse)
{
	const FDetachmentTransformRules DetachRules(EDetachmentRule::KeepWorld, true);
	Weapon->DetachFromComponent(DetachRules);
	MulticastHandleDeath(DeathImpulse);
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation() const
{
	return AttackMontages;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation() const
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) const
{
	for (FTaggedMontage TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag == MontageTag)
		{
			return TaggedMontage;
		}
	}
	return FTaggedMontage{};
}

int32 AAuraCharacterBase::GetMinionCount_Implementation() const
{
	return MinionCount;
}

void AAuraCharacterBase::IncrementMinionCount_Implementation(const int32 Amount)
{
	MinionCount += Amount;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation() const
{
	return CharacterClass;
}

FOnASCRegisteredSignature& AAuraCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegisteredDelegate;
}

FOnDeathSignature& AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeapon_Implementation() const
{
	if (Weapon == nullptr || Weapon->GetSkeletalMeshAsset() == nullptr)
	{
		return GetMesh();
	}
	
	return Weapon;
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector& DeathImpulse)
{
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this, DeathSound,
			GetActorLocation(),
			GetActorRotation()
		);
	}
	
	if (Weapon)
	{
		Weapon->SetSimulatePhysics(true);
		Weapon->SetEnableGravity(true);
		Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		Weapon->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);
	}

	if (USkeletalMeshComponent* CharacterMesh = GetMesh())
	{
		CharacterMesh->SetSimulatePhysics(true);
		CharacterMesh->SetEnableGravity(true);
		CharacterMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		CharacterMesh->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		CharacterMesh->AddImpulse(DeathImpulse, NAME_None, true);
	}
	
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	// Stop all montages
	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->CurrentMontageStop();
		AbilitySystemComponent->CancelAllAbilities();
	}
	
	// 1) Stop movement completely
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
		MoveComp->Velocity = FVector::ZeroVector; // ensure no residual velocity drags the camera
		MoveComp->SetComponentTickEnabled(false); // prevents further movement updates
	}
	
	// 2) Stop any looping audio on the character (run loops, metasounds, etc.)
	TArray<UAudioComponent*> AudioComponents;
	GetComponents<UAudioComponent>(AudioComponents);
	for (UAudioComponent* AC : AudioComponents)
	{
		if (IsValid(AC) && AC->IsPlaying())
		{
			AC->Stop();
		}
	}
	
	// Eventually remove the debuff
	if (BurnDebuffComponent)
	{
		BurnDebuffComponent->Deactivate();
	}
	
	// Eventually remove the debuff
	if (StunDebuffComponent)
	{
		StunDebuffComponent->Deactivate();
	}
	
	Dissolve();
	bDead = true;
	OnDeathDelegate.Broadcast(this);
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
	// Extended on child classes
}

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const
{
	checkf(GameplayEffectClass != nullptr, TEXT("GameplayEffectClass is not set"));
	checkf(GetAbilitySystemComponent(), TEXT("AbilitySystemComponent is not set"));
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	const int32 PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(this);
	ApplyEffectToSelf(DefaultPrimaryAttributes, PlayerLevel);
	ApplyEffectToSelf(DefaultSecondaryAttributes, PlayerLevel);
	ApplyEffectToSelf(DefaultVitalAttributes, PlayerLevel);
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		const int32 NumMaterials = GetMesh()->GetNumMaterials();
		for (int i = 0; i < NumMaterials; ++i)
		{
			GetMesh()->SetMaterial(i, DynamicMatInst);
		}
		StartDissolveTimeline(DynamicMatInst);
	}
	
	if (Weapon && IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* WeaponDynamicMatInst = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		const int32 NumMaterials = Weapon->GetNumMaterials();
		for (int i = 0; i < NumMaterials; ++i)
		{
			Weapon->SetMaterial(i, WeaponDynamicMatInst);
		}
		StartWeaponDissolveTimeline(WeaponDynamicMatInst);
	}
}

/**
 * @see AAuraCharacter::PossessedBy
 */
void AAuraCharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority()) return;

	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent());
	if (!AuraASC) return;

	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}
