#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "RTSAbilitySystemGlobals.generated.h"

/**
 *
 */
UCLASS()
class ORDERSABILITIES_API URTSAbilitySystemGlobals : public UAbilitySystemGlobals
{
public:
	FGameplayAbilityActorInfo* AllocAbilityActorInfo() const override;
	FGameplayEffectContext* AllocGameplayEffectContext() const override;
private:
	GENERATED_BODY()

public:
};