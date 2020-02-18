// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "RTSBTTask_RTSMoveTo.generated.h"

UCLASS()
class ORDERSABILITIES_API URTSBTTask_RTSMoveTo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	UPROPERTY(EditAnywhere)
	float OverrideOrderRange = -1.0f;
	
	URTSBTTask_RTSMoveTo();
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	FVector GetTargetLocation(UBlackboardComponent* Blackboard, AActor* TargetAsActor);
	bool IsInRange(APawn* Pawn, UBlackboardComponent* Blackboard, const FVector& TargetLocation, AActor* TargetAsActor);
};
