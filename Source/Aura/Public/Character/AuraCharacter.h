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
	virtual int32 GetPlayerLevel_Implementation() const override;
	// End ICombatInterface
	
	// Start IPlayerInterface
	virtual void AddToXP_Implementation(const int32 InXP) override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(const int32 InXP) const override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetAttributePointsReward_Implementation(const int32 InLevel) const override;
	virtual int32 GetSpellPointsReward_Implementation(const int32 InLevel) const override;
	virtual void AddToPlayerLevel_Implementation(const int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(const int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(const int32 InSpellPoints) override;
	// End IPlayerInterface
	
	
	
private:
	virtual void InitAbilityActorInfo() override;
	
};
