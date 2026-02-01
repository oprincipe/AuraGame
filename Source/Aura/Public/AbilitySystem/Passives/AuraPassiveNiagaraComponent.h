// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AuraPassiveNiagaraComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraPassiveNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	UAuraPassiveNiagaraComponent();

	UPROPERTY(EditDefaultsOnly, Category = "Passive Spell")
	FGameplayTag PassiveSpellTag;
	
	
protected:
	virtual void BeginPlay() override;
	
	void OnPassiveActivate(const FGameplayTag& AbilityTag, const bool bActivate);
	
};
