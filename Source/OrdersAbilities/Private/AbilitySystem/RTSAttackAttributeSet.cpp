#include "AbilitySystem/RTSAttackAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystem/RTSGlobalTags.h"


const float URTSAttackAttributeSet::MAX_DAMAGE = 10000.0f;
const float URTSAttackAttributeSet::MAX_AttackRate = 10000.0f;
const float URTSAttackAttributeSet::MAX_RANGE = 10000.0f;
const float URTSAttackAttributeSet::MAX_OUTGOING_DAMAGE_MULTIPLIER = 10000.0f;


void URTSAttackAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME(UMyAttributeSet, MyAttribute);
    // Chances are this is how you would ordinarily do it, however in the case of attributes this'll lead
    // to confusing and annoying replication errors, usually involving clientside ability prediction.
    // https://wiki.unrealengine.com/GameplayAbilities_and_You#AttributeSet
    // This is how it is done properly for attributes.
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, AttackDamageLow, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, AttackDamageHigh, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, AttackRate, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, Range, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, OutgoingDamageMultiplier, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSAttackAttributeSet, CooldownReduction, COND_None, REPNOTIFY_Always);
}

URTSAttackAttributeSet::URTSAttackAttributeSet()
{
    AttackDamageLow = 1.0f;
    AttackDamageHigh = 2.0f;
	AttackRate = 2.0f;
	Range = 125.0f;
	OutgoingDamageMultiplier = 1.0f;
    CooldownReduction = 0.0f;
}

bool URTSAttackAttributeSet::ShouldInitProperty(bool FirstInit, UProperty* PropertyToInit) const
{
    // We do not want the health property to change when the attribute sets properties where initialized using a curve
    // table.
    return (PropertyToInit != GetOutgoingDamageMultiplierAttribute().GetUProperty());
}

void URTSAttackAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
    if (Attribute == GetAttackDamageLowAttribute())
    {
        // Note that we can use game play effects to heal units so we can clamp this value with min == 0.
        NewValue = FMath::Clamp(NewValue, 0.0f, MAX_DAMAGE);
    }

    if (Attribute == GetAttackRateAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, MAX_AttackRate);
    }

    if (Attribute == GetRangeAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, MAX_RANGE);
    }

    if (Attribute == GetOutgoingDamageMultiplierAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, MAX_OUTGOING_DAMAGE_MULTIPLIER);
    }
}

void URTSAttackAttributeSet::PostInitializeProperties(bool bInitialInit)
{
    if (bInitialInit)
    {
        OutgoingDamageMultiplier = 1.0f;
    }
}

void URTSAttackAttributeSet::GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const
{
    OutStatusTags.AddTag(URTSGlobalTags::Status_Permanent_CanAttack());
}
