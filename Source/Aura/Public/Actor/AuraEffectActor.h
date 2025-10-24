// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	/** Instant effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	/** End Instant effects */


	/** Duration effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	/** End Duration effects */
	
	
	/** Infinite effects */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	/** End Infinite effects */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bDestroyOnEffectRemoval = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	float ActorLevel = 1.0f;

#pragma region Functions
	
	UFUNCTION(BlueprintCallable, Category="AuraEffectActor")
	void ApplyEffectToTarget(AActor* TargetActor, const TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable, Category="AuraEffectActor")
	void OnOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="AuraEffectActor")
	void OnEndOverlap(AActor* TargetActor);

#pragma endregion
};
