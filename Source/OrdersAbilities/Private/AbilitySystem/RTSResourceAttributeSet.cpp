#include "AbilitySystem/RTSResourceAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystem/RTSGlobalTags.h"

void URTSResourceAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URTSResourceAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URTSResourceAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URTSResourceAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
}

URTSResourceAttributeSet::URTSResourceAttributeSet()
{
	Mana = MaxMana = 100.0f;
	ManaRegen = 2.0f;
}

void URTSResourceAttributeSet::Init()
{
	BIND_ATTRIBUTE_MAXVALUE_CHANGED(URTSResourceAttributeSet, MaxMana);
}

void URTSResourceAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	if (Attribute == GetManaAttribute())
    {
        NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
    }

	if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(0.0f, GetMaxMana());
	}
}
