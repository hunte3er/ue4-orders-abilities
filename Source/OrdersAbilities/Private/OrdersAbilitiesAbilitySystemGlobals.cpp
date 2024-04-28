// Copyright 2020 Dan Kestranek.

#include "OrdersAbilitiesAbilitySystemGlobals.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/GameplayAbilityTypes.h"


FGameplayAbilityActorInfo* UOrdersAbilitiesAbilitySystemGlobals::AllocAbilityActorInfo() const
{
	return new FGameplayAbilityActorInfo();
}

FGameplayEffectContext* UOrdersAbilitiesAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FGameplayEffectContext();
}
