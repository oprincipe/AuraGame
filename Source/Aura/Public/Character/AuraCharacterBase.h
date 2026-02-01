// Copyright NattySoft

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAuraDebuffNiagaraComponent;
class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Start IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// End IAbilitySystemInterface

	// Start ICombatInterface
	virtual int32 GetPlayerLevel_Implementation() const override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual FName GetWeaponSocketName_Implementation() override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() const override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() const override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) const override;
	virtual int32 GetMinionCount_Implementation() const override;
	virtual void IncrementMinionCount_Implementation(const int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() const override;
	virtual FOnASCRegisteredSignature& GetOnASCRegisteredDelegate() override;
	virtual FOnDeathSignature& GetOnDeathDelegate() override;
	virtual USkeletalMeshComponent* GetWeapon_Implementation() const override;
	virtual bool IsBeingShocked_Implementation() const override;
	virtual void SetIsBeingShocked_Implementation(const bool IsBeingShocked) override;
	// End ICombatInterface
	
	FOnASCRegisteredSignature OnASCRegisteredDelegate;
	FOnDeathSignature OnDeathDelegate;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath(const FVector& DeathImpulse);
	
	UPROPERTY(ReplicatedUsing=OnRep_Burned, BlueprintReadOnly)
	bool bIsBurned = false;
	
	UPROPERTY(ReplicatedUsing=OnRep_Stunned, BlueprintReadOnly)
	bool bIsStunned = false;
	
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bIsBeenShocked = false;
	
	UFUNCTION() virtual void OnRep_Burned();
	UFUNCTION() virtual void OnRep_Stunned();
	
protected:
    virtual void BeginPlay() override;
	
    bool bDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
	
	/** Combat Variables */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandSocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandSocketName;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	FName TailSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	UNiagaraSystem* BloodEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	USoundBase* DeathSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	float BaseWalkSpeed = 250.f;
	
	/** Minion Variables */
	int32 MinionCount = 0;
	
	/** Attributes Variables */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Materials")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Materials")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FX")
	TObjectPtr<UAuraDebuffNiagaraComponent> BurnDebuffComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="FX")
	TObjectPtr<UAuraDebuffNiagaraComponent> StunDebuffComponent;
	
	/** Functions */
	virtual void InitAbilityActorInfo();

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	
	void Dissolve();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UPROPERTY() TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY() TObjectPtr<UAttributeSet> AttributeSet;

	void AddCharacterAbilities() const;
	virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
public:
    UAttributeSet* GetAttributeSet() const { return AttributeSet; }

    
private:
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;
	
    UPROPERTY(EditAnywhere, Category="Combat")
    TObjectPtr<UAnimMontage> HitReactMontage;
    
    UPROPERTY(EditAnywhere, Category="Combat")
    TArray<FTaggedMontage> AttackMontages;
};
