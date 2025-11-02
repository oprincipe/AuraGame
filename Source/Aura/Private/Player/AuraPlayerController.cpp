// Copyright NattySoft


#include "Player/AuraPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	
}

void AAuraPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CursorTrace();
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor. There are several scenarios:
	 * A. LastActor is null && ThisActor is null
	 *	- Do nothing
	 * B. LastActor is null && ThisActor is not null
	 *  - Highlight ThisActor
	 * C. LastActor is not null && ThisActor is null
	 *  - UnHighlight LastActor
	 * D. LastActor is not null && ThisActor is not null, but LastActor != ThisActor
	 *  - UnHighlight LastActor
	 *  - Highlight ThisActor
	 * E. LastActor is not null && ThisActor is not null, but LastActor == ThisActor
	 *  - Do nothing
	 */
	HandleActorHighlighting();
}

void AAuraPlayerController::HandleActorHighlighting()
{
	// Case A: Both actors are null - do nothing
	if (LastActor == nullptr && ThisActor == nullptr)
	{
		return;
	}
	
	// Case B: Only ThisActor is valid - highlight it
	if (LastActor == nullptr && ThisActor != nullptr)
	{
		ThisActor->HighlightActor();
		return;
	}
	
	// Case C: Only LastActor is valid - unhighlight it
	if (LastActor != nullptr && ThisActor == nullptr)
	{
		LastActor->UnHighlightActor();
		return;
	}
	
	// Case D & E: Both actors are valid
	if (LastActor != nullptr && ThisActor != nullptr)
	{
		if (LastActor != ThisActor)
		{
			// Case D: Different actors - switch highlighting
			LastActor->UnHighlightActor();
			ThisActor->HighlightActor();
		}
		// Case E: Same actor - do nothing (implicit)
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	checkf(AuraContext, TEXT("AuraContext is not set"));

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		SubSystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

	// Custom method to bind action with GameplayTag
	AuraInputComponent->BindAbilityActions(InputConfig, this, &AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputTagHeld);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = FRotator::ZeroRotator;
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardVector, InputAxisVector.X, false);
		ControlledPawn->AddMovementInput(RightVector, InputAxisVector.Y, false);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, FString::Printf(TEXT("Ability Input Tag Pressed: %s"), *InputTag.ToString()));
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, FString::Printf(TEXT("Ability Input Tag Released: %s"), *InputTag.ToString()));
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, FString::Printf(TEXT("Ability Input Tag Held: %s"), *InputTag.ToString()));
}