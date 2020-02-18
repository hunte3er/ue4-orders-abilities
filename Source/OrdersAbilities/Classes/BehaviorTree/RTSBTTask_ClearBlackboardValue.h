// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RTSBTTask_ClearBlackboardValue.generated.h"

/**
 * 
 */
UCLASS()
class ORDERSABILITIES_API URTSBTTask_ClearBlackboardValue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = ClearBlackboard, EditAnywhere)
	FBlackboardKeySelector BlackboardKey;
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
