#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/RTSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RTSStatsAttributeSet.generated.h"

/**
 * Contains attributes about the attack of a unit or building.
 */
UCLASS(BlueprintType)
class ORDERSABILITIES_API URTSStatsAttributeSet : public URTSAttributeSet
{
    GENERATED_BODY()

    //  YOU ARE NOT ALLOWED TO CHANGE THE ATTRIBUTE VALUES!
    //
    //  This attributes are only PUBLIC because of the 'DEFINE_ATTRIBUTE_CAPTUREDEF' macro that is used when
    //  implementing a 'UGameplayEffectExecutionCalculation'.This attribute values should only be changed using
    //  GameplayEffects.
public:
	URTSStatsAttributeSet();

    /** Str Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Str)
	FGameplayAttributeData Str;
	ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, Str)

	UFUNCTION()
	void OnRep_Str(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, Str, OldValue); }

    /** Dex Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Dex)
    FGameplayAttributeData Dex;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, Dex)

    UFUNCTION()
    void OnRep_Dex(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, Dex, OldValue); }

    /** Int Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Int)
    FGameplayAttributeData Int;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, Int)

	UFUNCTION()
	void OnRep_Int(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, Int, OldValue); }

    /** StrGrowth Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_StrGrowth)
    FGameplayAttributeData StrGrowth;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, StrGrowth)

    UFUNCTION()
    void OnRep_StrGrowth(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, StrGrowth, OldValue); }

    /** DexGrowth Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_DexGrowth)
    FGameplayAttributeData DexGrowth;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, DexGrowth)

    UFUNCTION()
    void OnRep_DexGrowth(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, DexGrowth, OldValue); }

    /** IntGrowth Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_IntGrowth)
    FGameplayAttributeData IntGrowth;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, IntGrowth)

    UFUNCTION()
    void OnRep_IntGrowth(const FGameplayAttributeData& OldValue) { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, IntGrowth, OldValue); }

    //~ Begin UAttributeSet Interface
    virtual bool ShouldInitProperty(bool FirstInit, FProperty* PropertyToInit) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    //~ End UAttributeSet Interface

    //~ Begin URTSAttributeSet Interface
    virtual void GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const override;
    //~ End URTSAttributeSet Interface
};
