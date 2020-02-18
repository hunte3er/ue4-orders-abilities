// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RTSBTDecorator_IsUnderAttack.generated.h"

/**
 * 
 */
UCLASS()
class ORDERSABILITIES_API URTSBTDecorator_IsUnderAttack : public UBTDecorator
{
	GENERATED_BODY()

public:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
