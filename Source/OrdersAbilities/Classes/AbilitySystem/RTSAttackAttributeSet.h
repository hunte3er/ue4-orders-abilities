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
    static const float MAX_COOLDOWN;
    static const float MAX_RANGE;
    static const float MAX_OUTGOING_DAMAGE_MULTIPLIER;

	URTSAttackAttributeSet();

    /** AttackDamageLow Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_AttackDamageLow)
	FGameplayAttributeData AttackDamageLow;
	ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, AttackDamageLow)

	UFUNCTION()
	void OnRep_AttackDamageLow() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, AttackDamageLow); }

    /** AttackDamageHigh Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_AttackDamageHigh)
    FGameplayAttributeData AttackDamageHigh;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, AttackDamageHigh)

    UFUNCTION()
    void OnRep_AttackDamageHigh() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, AttackDamageHigh); }

    /** Time before the attack can be used again, in seconds. */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Cooldown)
    FGameplayAttributeData Cooldown;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, Cooldown)

	UFUNCTION()
	void OnRep_Cooldown() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, Cooldown); }

    /** Attack range, in cm. */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Range)
    FGameplayAttributeData Range;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, Range)

    UFUNCTION()
    void OnRep_Range() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, Range); }

    /**
     * The OutgoingDamageMultiplier of a target is multiplied with incoming damage to modify the loss of hit points of
     * the attacked unit.
     */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_OutgoingDamageMultiplier)
    FGameplayAttributeData OutgoingDamageMultiplier;
    ATTRIBUTE_ACCESSORS(URTSAttackAttributeSet, OutgoingDamageMultiplier)

    UFUNCTION()
    void OnRep_OutgoingDamageMultiplier() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSAttackAttributeSet, OutgoingDamageMultiplier); }

    //~ Begin UAttributeSet Interface
    virtual bool ShouldInitProperty(bool FirstInit, UProperty* PropertyToInit) const override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    //~ End UAttributeSet Interface

    //~ Begin URTSAttributeSet Interface
    virtual void PostInitializeProperties(bool bInitialInit) override;
    virtual void GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const override;
    //~ End URTSAttributeSet Interface
};
