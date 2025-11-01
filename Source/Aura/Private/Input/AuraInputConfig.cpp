// Copyright NattySoft


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	const bool bLogNotFound) const
{
	for (const auto& [InputAction, InputTag] : InputActions)
	{
		if (InputTag.MatchesTagExact(InputTag))
		{
			return InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Input action not found for tag: [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
