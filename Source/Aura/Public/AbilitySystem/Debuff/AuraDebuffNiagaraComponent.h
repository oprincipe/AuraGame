// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "AuraDebuffNiagaraComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AURA_API UAuraDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	
	UAuraDebuffNiagaraComponent();
	
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;
	
protected:
	virtual void BeginPlay() override;
	
	void DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	UFUNCTION() void OnOwnerDeath(AActor* DeadActor);
};
