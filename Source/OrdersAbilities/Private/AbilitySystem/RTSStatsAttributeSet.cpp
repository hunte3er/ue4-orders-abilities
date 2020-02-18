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
    if (Attribute == GetStrAttribute())
    {
        ApplyStatChange(NewValue - GetStr(), StrEffectClass, StrEffectHandle);
    }

    if (Attribute == GetDexAttribute())
    {
        ApplyStatChange(NewValue - GetDex(), DexEffectClass, DexEffectHandle);
    }

    if (Attribute == GetIntAttribute())
    {
        ApplyStatChange(NewValue - GetInt(), IntEffectClass, IntEffectHandle);
    }
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

void URTSStatsAttributeSet::PostInitializeProperties(bool bInitialInit)
{
    if (bInitialInit)
    {
        if (IsValid(StrEffectClass))
        {
            UGameplayEffect* StrEffect = StrEffectClass.GetDefaultObject();
            FGameplayEffectSpecHandle SpecHandle = UAbilitySystemBlueprintLibrary::MakeSpecHandle(StrEffect, GetOwningActor(), GetOwningActor());
            SpecHandle.Data->StackCount = FMath::FloorToInt(GetStr());
            StrEffectHandle = GetOwningAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }

        if (IsValid(DexEffectClass))
        {
            UGameplayEffect* DexEffect = DexEffectClass.GetDefaultObject();
            FGameplayEffectSpecHandle SpecHandle = UAbilitySystemBlueprintLibrary::MakeSpecHandle(DexEffect, GetOwningActor(), GetOwningActor());
            SpecHandle.Data->StackCount = FMath::FloorToInt(GetDex());
            DexEffectHandle = GetOwningAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }

        if (IsValid(IntEffectClass))
        {
            UGameplayEffect* IntEffect = IntEffectClass.GetDefaultObject();
            FGameplayEffectSpecHandle SpecHandle = UAbilitySystemBlueprintLibrary::MakeSpecHandle(IntEffect, GetOwningActor(), GetOwningActor());
            SpecHandle.Data->StackCount = FMath::FloorToInt(GetInt());
            IntEffectHandle = GetOwningAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
        }
    }
}

void URTSStatsAttributeSet::GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const
{
    OutStatusTags.AddTag(URTSGlobalTags::Status_Permanent_HasStats());
}

void URTSStatsAttributeSet::ApplyStatChange(float StatChange, TSubclassOf<UGameplayEffect> StatEffectClass, const FActiveGameplayEffectHandle& EffectHandle)
{
    UGameplayEffect* StatEffect = StatEffectClass.GetDefaultObject();
	if (!StatEffect)
	{
		UE_LOG(LogRTS, Error, TEXT("%s: Invalid Stat Effect"), TEXT(__FUNCTION__));
        return;
	}
	
	if (StatChange > 0)
	{		
        FGameplayEffectSpecHandle SpecHandle = UAbilitySystemBlueprintLibrary::MakeSpecHandle(StatEffect, GetOwningActor(), GetOwningActor());
        SpecHandle.Data->StackCount = FMath::FloorToInt(StatChange);
        GetOwningAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
    else if (StatChange < 0)
    {
        GetOwningAbilitySystemComponent()->RemoveActiveGameplayEffect(EffectHandle, FMath::FloorToInt(FMath::Abs(StatChange)));
    }
}
