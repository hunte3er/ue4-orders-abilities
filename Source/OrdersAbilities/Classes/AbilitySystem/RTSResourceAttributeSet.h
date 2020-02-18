#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RTSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RTSResourceAttributeSet.generated.h"

/**
 * Contains attributes about the Resource of a unit or building.
 */
UCLASS(BlueprintType)
class ORDERSABILITIES_API URTSResourceAttributeSet : public URTSAttributeSet
{
    GENERATED_BODY()

public:
	URTSResourceAttributeSet();

	/** Mana Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Resource", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(URTSResourceAttributeSet, Mana)

	UFUNCTION()
	void OnRep_Mana() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSResourceAttributeSet, Mana); }

	ATTRIBUTE_MAXVALUE_CHANGED(URTSResourceAttributeSet, Mana)

	/** MaxMana Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Resource", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(URTSResourceAttributeSet, MaxMana)

	UFUNCTION()
	void OnRep_MaxMana() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSResourceAttributeSet, MaxMana); }
	
	/** ManaRegen Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Resource", ReplicatedUsing = OnRep_ManaRegen)
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(URTSResourceAttributeSet, ManaRegen)
	
	UFUNCTION()
	void OnRep_ManaRegen() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSResourceAttributeSet, ManaRegen); }

    //~ Begin UAttributeSet Interface
	virtual void Init() override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    //~ End UAttributeSet Interface
};
