#include "AbilitySystem/RTSCurrencyAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystem/RTSGlobalTags.h"

void URTSCurrencyAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(URTSCurrencyAttributeSet, Currency, COND_None, REPNOTIFY_Always);
}

URTSCurrencyAttributeSet::URTSCurrencyAttributeSet()
{
	Currency = 100.0f;
    Income = 5.0f;
}

void URTSCurrencyAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);
    // if (Attribute == GetCurrencyAttribute())
    // {
    //     NewValue = FMath::Max(NewValue, 0.0f);
    // }
}
