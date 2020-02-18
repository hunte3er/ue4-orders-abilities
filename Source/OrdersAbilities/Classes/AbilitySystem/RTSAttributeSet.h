#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayEffectTypes.h"
#include "RTSAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define ATTRIBUTE_MAXVALUE_CHANGED(ClassName, PropertyName) \
	void OnMax##PropertyName##Changed(const FOnAttributeChangeData& ChangeData) \
	{ \
		const float OldPercent = Get##PropertyName() / ChangeData.OldValue; \
		const float NewValue = OldPercent * ChangeData.NewValue; \
		Set##PropertyName(NewValue); \
	}

#define BIND_ATTRIBUTE_MAXVALUE_CHANGED(ClassName, PropertyName) \
	GetOwningAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Get##PropertyName##Attribute()).AddUObject(this, &ClassName::On##PropertyName##Changed);

/**
 * RTS base class for ability system attribute sets
 */
UCLASS(BlueprintType, Abstract)
class ORDERSABILITIES_API URTSAttributeSet : public UAttributeSet
{
    GENERATED_BODY()
	
public:
    UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;

    UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<UGameplayEffect>> EffectsToApply;

    virtual void Init();
	
    /** Always called after the attribute values where initialized using a curve table. */
    virtual void PostInitializeProperties(bool bInitialInit);

    /** Gets the status tags this attribute set grants to its owner. */
    virtual void GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const;

    /** Grant attribute set abilities. */
    void GrantAbilities() const;
};
