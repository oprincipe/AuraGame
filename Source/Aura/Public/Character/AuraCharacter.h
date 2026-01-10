// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Start ICombatInterface
	virtual int32 GetPlayerLevel() const override;
	// End ICombatInterface
	
	// Start IPlayerInterface
	virtual void AddToXP_Implementation(const int32 InXP) override;
	// End IPlayerInterface
	
	
	
private:
	virtual void InitAbilityActorInfo() override;
	
};
