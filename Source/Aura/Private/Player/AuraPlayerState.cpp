// Copyright NattySoft


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	
	SetNetUpdateFrequency(100.f);
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::AddToXP(const int32 InXP)
{
	XP += InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void AAuraPlayerState::AddToLevel(const int32 InLevel)
{
	Level += InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::AddToAttributePoints(const int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointsChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::AddToSpellPoints(const int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointsChangeDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::SetLevel(const int32 InLevel)
{
	Level = InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::SetXP(const int32 InXP)
{
	XP = InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_Level(const int32& OldLevel) const
{
	if (Level <= 1) return;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AAuraPlayerState::OnRep_XP(const int32 OldXP) const
{
	OnXPChangeDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(const int32 OldAttributePoints) const
{
	OnAttributePointsChangeDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(const int32 OldSpellPoints) const
{
	OnSpellPointsChangeDelegate.Broadcast(SpellPoints);
}
