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
    UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> StrEffectClass;
    FActiveGameplayEffectHandle StrEffectHandle;

    UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DexEffectClass;
    FActiveGameplayEffectHandle DexEffectHandle;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameplayEffect> IntEffectClass;
    FActiveGameplayEffectHandle IntEffectHandle;
	
	URTSStatsAttributeSet();

    /** Str Attribute */
	UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Str)
	FGameplayAttributeData Str;
	ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, Str)

	UFUNCTION()
	void OnRep_Str() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, Str); }

    /** Dex Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Dex)
    FGameplayAttributeData Dex;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, Dex)

    UFUNCTION()
    void OnRep_Dex() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, Dex); }

    /** Int Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_Int)
    FGameplayAttributeData Int;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, Int)

	UFUNCTION()
	void OnRep_Int() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, Int); }

    /** StrGrowth Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_StrGrowth)
    FGameplayAttributeData StrGrowth;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, StrGrowth)

    UFUNCTION()
    void OnRep_StrGrowth() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, StrGrowth); }

    /** DexGrowth Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_DexGrowth)
    FGameplayAttributeData DexGrowth;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, DexGrowth)

    UFUNCTION()
    void OnRep_DexGrowth() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, DexGrowth); }

    /** IntGrowth Attribute */
    UPROPERTY(BlueprintReadOnly, Category = "Attributes|Attack", ReplicatedUsing = OnRep_IntGrowth)
    FGameplayAttributeData IntGrowth;
    ATTRIBUTE_ACCESSORS(URTSStatsAttributeSet, IntGrowth)

    UFUNCTION()
    void OnRep_IntGrowth() { GAMEPLAYATTRIBUTE_REPNOTIFY(URTSStatsAttributeSet, IntGrowth); }

    //~ Begin UAttributeSet Interface
    virtual bool ShouldInitProperty(bool FirstInit, UProperty* PropertyToInit) const override;
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
    virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
    virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
    //~ End UAttributeSet Interface

    //~ Begin URTSAttributeSet Interface
    virtual void PostInitializeProperties(bool bInitialInit) override;
    virtual void GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const override;
    //~ End URTSAttributeSet Interface

    void ApplyStatChange(float StatChange, TSubclassOf<UGameplayEffect> StatEffectClass, const FActiveGameplayEffectHandle& EffectHandle);
};
