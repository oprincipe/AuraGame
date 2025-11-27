// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UDamageTextWidgetComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class IEnemyInterface;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
private:
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category="Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UAuraInputConfig> InputConfig;

	TScriptInterface<IEnemyInterface> LastActor;
	TScriptInterface<IEnemyInterface> ThisActor;
	FHitResult CursorHit;

	UPROPERTY(EditDefaultsOnly, Category="Widget Components")
	TSubclassOf<UDamageTextWidgetComponent> DamageTextWidgetComponentClass;
	
	/** Movement logic */
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	UPROPERTY(EditDefaultsOnly) float AutoRunAcceptanceRadius = 50.f;
	UPROPERTY(VisibleAnywhere) TObjectPtr<USplineComponent> Spline;
	/** End movement logic */
	void Move(const FInputActionValue& InputActionValue);
	
	/** Shift logic */
	bool bShiftDown = false;
	void ShiftPressed() { bShiftDown = true; };
	void ShiftReleased() { bShiftDown = false; };
	/** End Shift logic */
	
	void AutoRun();
	void CursorTrace();
	void HandleActorHighlighting() const;
	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
	UAuraAbilitySystemComponent* GetASC();
	
public:
	UFUNCTION(Client, Reliable)
	void Client_ShowDamageNumber(const float DamageAmount, ACharacter* TargetCharacter);
};
