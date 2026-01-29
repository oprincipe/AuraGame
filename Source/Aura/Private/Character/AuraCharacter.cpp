// Copyright NattySoft


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/AuraPlayerController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"


AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;
	
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;
	
	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	CharacterClass = ECharacterClass::Elementalist;
	
	BaseWalkSpeed = 600.f;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Init ability actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init ability actor info for the client
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_Stunned()
{
	if (UAuraAbilitySystemComponent* ASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		FGameplayTagContainer BlockedTags;
		const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			ASC->AddLooseGameplayTags(BlockedTags);
		}
		else
		{
			ASC->RemoveLooseGameplayTags(BlockedTags);
		}
	}
	
}

int32 AAuraCharacter::GetPlayerLevel_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	return AuraPlayerState->GetPlayerLevel();
}

FVector AAuraCharacter::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const USkeletalMeshComponent* WeaponMesh = Execute_GetWeapon(this);
	if (IsValid(WeaponMesh) && WeaponMesh->GetSkeletalMeshAsset())
	{
		return  WeaponMesh->GetSocketLocation(WeaponTipSocketName);
	}
	
	return Super::GetCombatSocketLocation_Implementation(MontageTag);
}

void AAuraCharacter::AddToXP_Implementation(const int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	AuraPlayerState->AddToXP(InXP);
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(const int32 InXP) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	Multicast_LevelUp_Particles();
}

void AAuraCharacter::Multicast_LevelUp_Particles_Implementation()
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(const int32 InLevel) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(const int32 InLevel) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[InLevel].SpellPointAward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(const int32 InPlayerLevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	AuraPlayerState->AddToLevel(InPlayerLevel);
	
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(const int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(const int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

void AAuraCharacter::AddAttributePoints_Implementation(const int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	checkf(AuraPlayerState, TEXT("PlayerState is not set"));
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnASCRegisteredDelegate.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraCharacter::StunTagChanged
		);
	
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(GetWorld()->GetFirstPlayerController()->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	InitializeDefaultAttributes();
}


