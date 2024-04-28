#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RTSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RTSAttackAttributeSet.generated.h"

/**
 * Contains attributes about the attack of a unit or building.
 */
UCLASS(BlueprintType)
class ORDERSABILITIES_API URTSAttackAttributeSet : public URTSAttributeSet
{
    GENERATED_BODY()

    //  YOU ARE NOT ALLOWED TO CHANGE THE ATTRIBUTE VALUES!
    //
    //  This attributes are only PUBLIC because of the 'DEFINE_ATTRIBUTE_CAPTUREDEF' macro that is used when
    //  implementing a 'UGameplayEffectExecutionCalculation'.This attribute values should only be changed using
    //  GameplayEffects.
public:
    static const float MAX_DAMAGE;
    static const float MAX_AttackRate;
    static const float MAX_RANGE;
    static const float MAX_OUTGOING_DAMAGE_MULTIPLIER;

	URTSAttackAttributeSet();

    /** AttackDamageLow Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_AttackDamageLow)
	FGameplayAttributeData AttackDamageLow;
	ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, AttackDamageLow)

	UFUNCTION()
	void OnRep_AttackDamageLow(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, AttackDamageLow, OldValue); }

    /** AttackDamageHigh Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_AttackDamageHigh)
    FGameplayAttributeData AttackDamageHigh;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, AttackDamageHigh)

    UFUNCTION()
    void OnRep_AttackDamageHigh(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, AttackDamageHigh, OldValue); }

    /** Time before the attack can be used again, in seconds. */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_AttackRate)
    FGameplayAttributeData AttackRate;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, AttackRate)

	UFUNCTION()
	void OnRep_AttackRate(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, AttackRate, OldValue); }

    /** Range Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Range)
    FGameplayAttributeData Range;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, Range)

    UFUNCTION()
    void OnRep_Range(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, Range, OldValue); }

    /** CooldownReduction Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_CooldownReduction)
    FGameplayAttributeData CooldownReduction;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, CooldownReduction)

    UFUNCTION()
    void OnRep_CooldownReduction(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, CooldownReduction, OldValue); }

    /**
     * The OutgoingDamageMultiplier of a target is multiplied with incoming damage to modify the loss of hit points of
     * the attacked unit.
     */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_OutgoingDamageMultiplier)
    FGameplayAttributeData OutgoingDamageMultiplier;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, OutgoingDamageMultiplier)

    UFUNCTION()
    void OnRep_OutgoingDamageMultiplier(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, OutgoingDamageMultiplier, OldValue); }

    //~ Begin UAttributeSet Interface
    virtual bool ShouldInitProperty(bool FirstInit, FProperty* PropertyToInit) const override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    //~ End UAttributeSet Interface

    //~ Begin URTSAttributeSet Interface
    virtual void PostInitializeProperties(bool bInitialInit) override;
    virtual void GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const override;
    //~ End URTSAttributeSet Interface
};
