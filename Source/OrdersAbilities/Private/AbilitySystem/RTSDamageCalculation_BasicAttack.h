// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "RTSDamageCalculation_BasicAttack.generated.h"

/**
 * 
 */
UCLASS()
class ORDERSABILITIES_API URTSDamageCalculation_BasicAttack : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
	public:
	// Constructor and overrides
	URTSDamageCalculation_BasicAttack();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
