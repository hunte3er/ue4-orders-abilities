// #pragma once
//
// #include "CoreMinimal.h"
// #include "AbilitySystem/RTSAttributeSet.h"
// #include "AbilitySystemComponent.h"
// #include "RTSManaAttributeSet.generated.h"
//
// // Uses macros from AttributeSet.h
// #define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
// 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
// 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
// 	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
// 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
//
//
// /**
//  * Contains attributes about the Mana of a unit or building.
//  */
// UCLASS(BlueprintType)
// class ORDERSABILITIES_API URTSManaAttributeSet : public URTSAttributeSet
// {
//     GENERATED_BODY()
//
// public:
// 	URTSManaAttributeSet();
//
// 	/** Mana Attribute */
// 	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Mana", ReplicatedUsing = OnRep_Mana)
// 	FGameplayAttributeData Mana;
// 	ATTRIBUTE_ACCESSORS(URTSManaAttributeSet, Mana)
//
// 	UFUNCTION()
// 	void OnRep_Mana() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSManaAttributeSet, Mana); }
//
// 	//ATTRIBUTE_MAXVALUE_CHANGED(URTSManaAttributeSet, Mana)
//
// 	/** MaxMana Attribute */
// 	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Mana", ReplicatedUsing = OnRep_MaxMana)
// 	FGameplayAttributeData MaxMana;
// 	ATTRIBUTE_ACCESSORS(URTSManaAttributeSet, MaxMana)
//
// 	UFUNCTION()
// 	void OnRep_MaxMana() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSManaAttributeSet, MaxMana); }
// 	
// 	/** ManaRegen Attribute */
// 	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Mana", ReplicatedUsing = OnRep_ManaRegen)
// 	FGameplayAttributeData ManaRegen;
// 	ATTRIBUTE_ACCESSORS(URTSManaAttributeSet, ManaRegen)
// 	
// 	UFUNCTION()
// 	void OnRep_ManaRegen() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSManaAttributeSet, ManaRegen); }
//
//     //~ Begin UAttributeSet Interface
// 	virtual void Init() override;
//     virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
//     //~ End UAttributeSet Interface
// };
