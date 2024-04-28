#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RTSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RTSDefenseAttributeSet.generated.h"

/**
 * Contains attributes about the Defense of a unit or building.
 */
UCLASS(BlueprintType)
class ORDERSABILITIES_API URTSDefenseAttributeSet : public URTSAttributeSet
{
    GENERATED_BODY()

    //  YOU ARE NOT ALLOWED TO CHANGE THE ATTRIBUTE VALUES!
    //
    //  This attributes are only PUBLIC because of the 'DEFINE_ATTRIBUTE_CAPTUREDEF' macro that is used when
    //  implementing a 'UGameplayEffectExecutionCalculation'.This attribute values should only be changed using
    //  GameplayEffects.
public:
    static const float MAX_HEALTH;
    static const float MAX_PHYSICAL_ARMOR;
    static const float MAX_MAGICAL_ARMOR;
    static const float MAX_INCOMING_DAMAGE_MULTIPLIER;

	URTSDefenseAttributeSet();

	/** Damage Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Defense", ReplicatedUsing = OnRep_Damage)
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(URTSDefenseAttributeSet, Damage)

	UFUNCTION()
	void OnRep_Damage(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSDefenseAttributeSet, Damage, OldValue); }

	/** Health Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Defense", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(URTSDefenseAttributeSet, Health)

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSDefenseAttributeSet, Health, OldValue); }

	/** Max Health Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Defense", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(URTSDefenseAttributeSet, MaxHealth)

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSDefenseAttributeSet, MaxHealth, OldValue); }

	/** PhysicalArmor Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Defense", ReplicatedUsing = OnRep_PhysicalArmor)
	FGameplayAttributeData PhysicalArmor;
	ATTRIBUTE_ACCESSORS(URTSDefenseAttributeSet, PhysicalArmor)

	UFUNCTION()
	void OnRep_PhysicalArmor(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSDefenseAttributeSet, PhysicalArmor, OldValue); }

	/** MagicalArmor Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Defense", ReplicatedUsing = OnRep_MagicalArmor)
	FGameplayAttributeData MagicalArmor;
	ATTRIBUTE_ACCESSORS(URTSDefenseAttributeSet, MagicalArmor)

	UFUNCTION()
	void OnRep_MagicalArmor(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSDefenseAttributeSet, MagicalArmor, OldValue); }

	/** IncomingDamageMultiplier Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Defense", ReplicatedUsing = OnRep_IncomingDamageMultiplier)
	FGameplayAttributeData IncomingDamageMultiplier;
	ATTRIBUTE_ACCESSORS(URTSDefenseAttributeSet, IncomingDamageMultiplier)

	UFUNCTION()
	void OnRep_IncomingDamageMultiplier(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSDefenseAttributeSet, IncomingDamageMultiplier, OldValue); }

    //~ Begin UAttributeSet Interface
    virtual bool ShouldInitProperty(bool FirstInit, FProperty* PropertyToInit) const override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    //~ End UAttributeSet Interface

    //~ Begin URTSAttributeSet Interface
	virtual void Init() override;
    virtual void PostInitializeProperties(bool bInitialInit) override;
    virtual void GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const override;
    //~ End URTSAttributeSet Interface

	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const;
	void HandleHealthLost(AActor* SourceActor, float HealthLost, const float HealthBeforeLost);

	void OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData);
};