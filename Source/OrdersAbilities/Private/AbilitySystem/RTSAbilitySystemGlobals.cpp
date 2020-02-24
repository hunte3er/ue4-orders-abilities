#include "AbilitySystem/RTSAbilitySystemGlobals.h"
#include "AbilitySystem/RTSAbilityTypes.h"

FGameplayAbilityActorInfo* URTSAbilitySystemGlobals::AllocAbilityActorInfo() const
{
	return new FRTSGameplayAbilityActorInfo();
}

FGameplayEffectContext* URTSAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRTSGameplayEffectContext();
}
