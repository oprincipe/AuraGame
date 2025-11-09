// Copyright NattySoft


#include "AbilitySystem/AbilityTasks/AuraTargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UAuraTargetDataUnderMouse* UAuraTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UAuraTargetDataUnderMouse* MyObj = NewAbilityTask<UAuraTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UAuraTargetDataUnderMouse::Activate()
{
	// Check if the ability is being activated on a locally controlled character
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled)
	{
		// If locally controlled, directly send mouse cursor data to the server
		SendMouseCursorData();
	}
	else
	{
		// For non-locally controlled characters (remote clients):
		
		// Get the handle that identifies this specific ability instance
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		
		// Get the prediction key used for network synchronization
		const FPredictionKey PredictionKey = GetActivationPredictionKey();
		
		// Set up a delegate to handle target data when it's received from the server
		AbilitySystemComponent.Get()->
			AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).
			AddUObject(this, &UAuraTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// Check if there's already replicated target data available
		const bool bCalledDelegates = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
		
		// If no target data is available yet, set the task to wait for it
		if (!bCalledDelegates)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UAuraTargetDataUnderMouse::SendMouseCursorData()
{
	// Get the player controller from the ability's actor info
	const APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC) return;

	// Perform line trace under mouse cursor to get the hit result
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	// Create a scoped prediction window for client-side prediction
	FScopedPredictionWindow ScopePrediction(AbilitySystemComponent.Get());
	
	// Create and fill target data with the hit result
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;

	// Create the target data handle and add the hit data
	FGameplayAbilityTargetDataHandle DataHandle;
	DataHandle.Add(Data);
	
	// Send target data to server for replication
	FGameplayTag ApplicationTag;
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		ApplicationTag,
		AbilitySystemComponent->ScopedPredictionKey
	);

	// Broadcast target data to listeners if allowed
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UAuraTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	// Consume the replicated target data and don't keep it cached
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	// Broadcast target data to listeners if allowed
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
