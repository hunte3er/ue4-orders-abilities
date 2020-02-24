#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "RTSAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FRTSGameplayAbilityActorInfo : public FGameplayAbilityActorInfo
{
    GENERATED_USTRUCT_BODY()
	
public:
    virtual ~FRTSGameplayAbilityActorInfo() {}
};

USTRUCT(BlueprintType)
struct FRTSGameplayEffectContext : public FGameplayEffectContext
{
    GENERATED_USTRUCT_BODY()

public:
    virtual ~FRTSGameplayEffectContext() {}
};