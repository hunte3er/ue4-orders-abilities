// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RTSBTTask_FaceTarget.generated.h"

/**
 * 
 */
UCLASS()
class ORDERSABILITIES_API URTSBTTask_FaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	URTSBTTask_FaceTarget();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	FVector GetTargetLocation(UBlackboardComponent* Blackboard);

	bool IsFacingTarget(APawn* Pawn, const FVector& TargetLocation);
};
