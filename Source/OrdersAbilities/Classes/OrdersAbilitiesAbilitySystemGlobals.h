// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "OrdersAbilitiesAbilitySystemGlobals.generated.h"

/**
 *
 */
UCLASS()
class ORDERSABILITIES_API UOrdersAbilitiesAbilitySystemGlobals : public UAbilitySystemGlobals
{
public:
	FGameplayAbilityActorInfo* AllocAbilityActorInfo() const override;
	FGameplayEffectContext* AllocGameplayEffectContext() const override;
private:
	GENERATED_BODY()

public:
};