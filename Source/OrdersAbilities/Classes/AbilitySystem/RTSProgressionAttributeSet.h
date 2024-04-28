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
	void OnRep_Level(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, Level, OldValue); }

    /** CurrentExp Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_CurrentExp)
    FGameplayAttributeData CurrentExp;
    ATTRIBUTE_ACCESSORS(URTSProgressionAttributeSet, CurrentExp)

    UFUNCTION()
    void OnRep_CurrentExp(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, CurrentExp, OldValue); }

    /** ExpReward Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_ExpReward)
    FGameplayAttributeData ExpReward;
    ATTRIBUTE_ACCESSORS(URTSProgressionAttributeSet, ExpReward)

	UFUNCTION()
	void OnRep_ExpReward(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, ExpReward, OldValue); }

    /** SkillPoints Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_SkillPoints)
    FGameplayAttributeData SkillPoints;
    ATTRIBUTE_ACCESSORS(URTSProgressionAttributeSet, SkillPoints)

    UFUNCTION()
    void OnRep_SkillPoints(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, SkillPoints, OldValue); }

    /** StatPoints Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_StatPoints)
    FGameplayAttributeData StatPoints;
    ATTRIBUTE_ACCESSORS(URTSProgressionAttributeSet, StatPoints)

    UFUNCTION()
    void OnRep_StatPoints(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSProgressionAttributeSet, StatPoints, OldValue); }

    //~ Begin UAttributeSet Interface
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    //~ End UAttributeSet Interface
};
