// Copyright NattySoft


#include "AbilitySystem/Passives/AuraPassiveNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"


UAuraPassiveNiagaraComponent::UAuraPassiveNiagaraComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = false;
	
}

void UAuraPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->ActivatePassiveEffectDelegate.AddUObject(this, &UAuraPassiveNiagaraComponent::OnPassiveActivate);
	}
	else if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
		{
			if (UAuraAbilitySystemComponent* AuraLambdaASC = Cast<UAuraAbilitySystemComponent>(ASC))
			{
				AuraLambdaASC->ActivatePassiveEffectDelegate.AddUObject(this, &UAuraPassiveNiagaraComponent::OnPassiveActivate);
			}
		});
	}
	
}

void UAuraPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, const bool bActivate)
{
	if (!AbilityTag.MatchesTagExact(PassiveSpellTag)) return;
	
	if (bActivate && !IsActive())
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}
