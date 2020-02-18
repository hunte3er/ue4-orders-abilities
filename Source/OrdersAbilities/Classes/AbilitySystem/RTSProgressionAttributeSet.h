#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RTSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RTSProgressionAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * Contains attributes about the attack of a unit or building.
 */
UCLASS(BlueprintType)
class ORDERSABILITIES_API URTSProgressionAttributeSet : public URTSAttributeSet
{
    GENERATED_BODY()

    //  YOU ARE NOT ALLOWED TO CHANGE THE ATTRIBUTE VALUES!
    //
    //  This attributes are only PUBLIC because of the 'DEFINE_ATTRIBUTE_CAPTUREDEF' macro that is used when
    //  implementing a 'UGameplayEffectExecutionCalculation'.This attribute values should only be changed using
    //  GameplayEffects.
public:
	URTSProgressionAttributeSet();

    /** Level Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Level)
	FGameplayAttributeData Level;
	ATTRIBUTE_ACCESSORS(URTSProgressionAttributeSet, Level)

	UFUNCTION()
	void OnRep_Level() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, Level); }

    /** CurrentExp Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_CurrentExp)
    FGameplayAttributeData CurrentExp;
    ATTRIBUTE_ACCESSORS(URTSProgressionAttributeSet, CurrentExp)

    UFUNCTION()
    void OnRep_CurrentExp() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, CurrentExp); }

    /** Time before the attack can be used again, in seconds. */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_ExpReward)
    FGameplayAttributeData ExpReward;
    ATTRIBUTE_ACCESSORS(URTSProgressionAttributeSet, ExpReward)

	UFUNCTION()
	void OnRep_ExpReward() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, ExpReward); }

    /** Attack SkillPoints, in cm. */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_SkillPoints)
    FGameplayAttributeData SkillPoints;
    ATTRIBUTE_ACCESSORS(URTSProgressionAttributeSet, SkillPoints)

    UFUNCTION()
    void OnRep_SkillPoints() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, SkillPoints); }

    /**
     * The Temporary2 of a target is multiplied with incoming damage to modify the loss of hit points of
     * the attacked unit.
     */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Temporary2)
    FGameplayAttributeData Temporary2;
    ATTRIBUTE_ACCESSORS(URTSProgressionAttributeSet, Temporary2)

    UFUNCTION()
    void OnRep_Temporary2() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, Temporary2); }

    //~ Begin UAttributeSet Interface
    virtual bool ShouldInitProperty(bool FirstInit, UProperty* PropertyToInit) const override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    //~ End UAttributeSet Interface

    //~ Begin URTSAttributeSet Interface
    virtual void PostInitializeProperties(bool bInitialInit) override;
    //~ End URTSAttributeSet Interface
};
