#include "AbilitySystem/RTSStatsAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystem/RTSGlobalTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/RTSAbilitySystemHelper.h"

#include "OrdersAbilities/OrdersAbilities.h"

void URTSStatsAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME(UMyAttributeSet, MyAttribute);
    // Chances are this is how you would ordinarily do it, however in the case of attributes this'll lead
    // to confusing and annoying replication errors, usually involving clientside ability prediction.
    // https://wiki.unrealengine.com/GameplayAbilities_and_You#AttributeSet
    // This is how it is done properly for attributes.
    DOREPLIFETIME_CONDITION_NOTIFY(URTSStatsAttributeSet, Str, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URTSStatsAttributeSet, Dex, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSStatsAttributeSet, Int, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSStatsAttributeSet, StrGrowth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSStatsAttributeSet, DexGrowth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSStatsAttributeSet, IntGrowth, COND_None, REPNOTIFY_Always);
}

URTSStatsAttributeSet::URTSStatsAttributeSet()
{
    Str = 5.0f;
    Dex = 5.0f;
	Int = 5.0f;
	StrGrowth = 1.0f;
	DexGrowth = 1.0f;
	IntGrowth = 1.0f;
}

bool URTSStatsAttributeSet::ShouldInitProperty(bool FirstInit, UProperty* PropertyToInit) const
{
    // We do not want the health property to change when the attribute sets properties where initialized using a curve
    // table.
    return (PropertyToInit != GetDexGrowthAttribute().GetUProperty());
}

void URTSStatsAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
}

void URTSStatsAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetStrAttribute() || Attribute == GetDexAttribute() || Attribute == GetIntAttribute())
	{
        NewValue = FMath::Max(NewValue, 0.0f);
	}
}

void URTSStatsAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// if (Data.EvaluatedData.Attribute == GetStrAttribute())
	// {
 //        ApplyStatChange(Data.EvaluatedData.Magnitude, StrEffectClass, StrEffectHandle);
	// }
 //
 //    if (Data.EvaluatedData.Attribute == GetDexAttribute())
 //    {
 //        ApplyStatChange(Data.EvaluatedData.Magnitude, DexEffectClass, DexEffectHandle);
 //    }
 //
 //    if (Data.EvaluatedData.Attribute == GetIntAttribute())
 //    {
 //        ApplyStatChange(Data.EvaluatedData.Magnitude, IntEffectClass, IntEffectHandle);
 //    }
}

void URTSStatsAttributeSet::GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const
{
    OutStatusTags.AddTag(URTSGlobalTags::Status_Permanent_HasStats());
}