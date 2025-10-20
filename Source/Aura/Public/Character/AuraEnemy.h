// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

#pragma region IEnemyInterface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
#pragma endregion

protected:
	virtual void BeginPlay() override;
};
