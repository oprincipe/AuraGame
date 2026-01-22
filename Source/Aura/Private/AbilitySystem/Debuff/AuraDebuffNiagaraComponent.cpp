// Copyright NattySoft


#include "AbilitySystem/Debuff/AuraDebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"


UAuraDebuffNiagaraComponent::UAuraDebuffNiagaraComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	bAutoActivate = false;
}

void UAuraDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UAuraDebuffNiagaraComponent::DebuffTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &UAuraDebuffNiagaraComponent::DebuffTagChanged);
		});
	}
	
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &UAuraDebuffNiagaraComponent::OnOwnerDeath);
	}
}

void UAuraDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	const bool bOwnerValid = IsValid(GetOwner());
	bool bOwnerAlive = true;
	if (bOwnerValid)
	{
		bOwnerAlive = GetOwner()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(GetOwner());
	}
	
	if (NewCount > 0 && bOwnerValid && bOwnerAlive) Activate();
	else Deactivate();
}

void UAuraDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
