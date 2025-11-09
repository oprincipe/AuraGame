// Copyright NattySoft


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Aura/Aura.h"
#include "Components/CapsuleComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

int32 AAuraCharacterBase::GetPlayerLevel() const
{
	// Extended on child classes
	return ICombatInterface::GetPlayerLevel();
}

FVector AAuraCharacterBase::GetCombatSocketLocation()
{
	if (IsValid(Weapon) && Weapon->GetSkeletalMeshAsset())
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	
	return GetMesh()->GetSocketLocation(WeaponTipSocketName);
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
	ApplyEffectToSelf(DefaultPrimaryAttributes, GetPlayerLevel());
	ApplyEffectToSelf(DefaultSecondaryAttributes, GetPlayerLevel());
	ApplyEffectToSelf(DefaultVitalAttributes, GetPlayerLevel());
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
}
