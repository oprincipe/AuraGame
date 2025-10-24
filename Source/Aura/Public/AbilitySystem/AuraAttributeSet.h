// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY(EditAnywhere) AActor* SourceAvatarActor = nullptr;
	UPROPERTY(EditAnywhere) AActor* TargetAvatarActor = nullptr;
	
	UPROPERTY(EditAnywhere) AController* SourceController = nullptr;
	UPROPERTY(EditAnywhere) AController* TargetController = nullptr;
	
	UPROPERTY(EditAnywhere) ACharacter* SourceCharacter = nullptr;
	UPROPERTY(EditAnywhere) ACharacter* TargetCharacter = nullptr;
	
	UPROPERTY(EditAnywhere) FGameplayEffectContextHandle EffectContextHandle;

	FEffectProperties(){}
	FEffectProperties(UAbilitySystemComponent* InASC, AActor* InSourceAvatarActor, AActor* InTargetAvatarActor,
		AController* InSourceController, AController* InTargetController,
		ACharacter* InSourceCharacter, ACharacter* InTargetCharacter, FGameplayEffectContextHandle InEffectContextHandle):
	SourceASC(InASC),
	SourceAvatarActor(InSourceAvatarActor),
	TargetAvatarActor(InTargetAvatarActor),
	SourceController(InSourceController),
	TargetController(InTargetController),
	SourceCharacter(InSourceCharacter),
	TargetCharacter(InTargetCharacter),
	EffectContextHandle(InEffectContextHandle)
	{};
};


/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	// Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	
	// Functions
	UFUNCTION() void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION() void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION() void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION() void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
	
};
