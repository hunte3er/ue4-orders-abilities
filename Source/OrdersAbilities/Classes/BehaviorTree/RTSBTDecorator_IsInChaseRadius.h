// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RTSBTDecorator_IsInChaseRadius.generated.h"

/**
 * 
 */
UCLASS()
class ORDERSABILITIES_API URTSBTDecorator_IsInChaseRadius : public UBTDecorator
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(EditAnywhere)
	float ChaseDistance = 1500.0f;
	
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
