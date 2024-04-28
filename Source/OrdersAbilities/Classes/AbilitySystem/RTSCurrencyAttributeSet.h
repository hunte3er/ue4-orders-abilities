#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RTSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RTSCurrencyAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/**
 * Contains attributes about the Currency of a unit or building.
 */
UCLASS(BlueprintType)
class ORDERSABILITIES_API URTSCurrencyAttributeSet : public URTSAttributeSet
{
    GENERATED_BODY()

public:
	URTSCurrencyAttributeSet();

	/** Currency Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Currency", ReplicatedUsing = OnRep_Currency)
	FGameplayAttributeData Currency;
	ATTRIBUTE_ACCESSORS(URTSCurrencyAttributeSet, Currency)

	UFUNCTION()
	void OnRep_Currency(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSCurrencyAttributeSet, Currency, OldValue); }

	/** Income Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Currency", ReplicatedUsing = OnRep_Currency)
	FGameplayAttributeData Income;
	ATTRIBUTE_ACCESSORS(URTSCurrencyAttributeSet, Income)
	
	UFUNCTION()
	void OnRep_Income(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSCurrencyAttributeSet, Income, OldValue); }

    //~ Begin UAttributeSet Interface
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    //~ End UAttributeSet Interface
};
