// Copyright NattySoft


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	for (TTuple<FGameplayTag, FScalableFloat> Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}
	
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(
	const TArray<FTaggedMontage>& TaggedMontages)
{
	if (TaggedMontages.IsEmpty()) return FTaggedMontage{};
	const int32 RandomIndex = FMath::RandRange(0, TaggedMontages.Num() - 1);
	return TaggedMontages[RandomIndex];
}

float UAuraDamageGameplayAbility::GetDamageByDamageType(const FGameplayTag& DamageType, const float InLevel) const
{
	checkf(DamageTypes.Contains(DamageType), TEXT("Damage type %s not found in DamageTypes map"), *DamageType.ToString());
	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
}
