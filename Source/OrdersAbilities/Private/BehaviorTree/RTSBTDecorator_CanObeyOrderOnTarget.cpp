// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/RTSBTDecorator_CanObeyOrderOnTarget.h"
#include "AIController.h"
#include "Orders/RTSOrderComponent.h"
#include "Orders/RTSOrderHelper.h"
#include "Orders/RTSBlackboardHelper.h"
#include "BehaviorTree/BlackboardComponent.h"

URTSBTDecorator_CanObeyOrderOnTarget::URTSBTDecorator_CanObeyOrderOnTarget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void URTSBTDecorator_CanObeyOrderOnTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TNodeInstanceMemory* DecoratorMemory = reinterpret_cast<TNodeInstanceMemory*>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalculateRawConditionValue(OwnerComp, NodeMemory);
}

bool URTSBTDecorator_CanObeyOrderOnTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();

	AActor* Target = URTSBlackboardHelper::GetBlackboardOrderTarget(BlackboardComp);
	FVector2D Location = FVector2D(URTSBlackboardHelper::GetBlackboardOrderLocation(BlackboardComp));
	FRTSOrderData OrderData(OrderType, OrderIndex, Target, Location);
	OrderData.bUseLocation = bUseLocation;

	bool CanObeyOrder = URTSOrderHelper::CheckOrder(Pawn, OrderData).IsEmpty();

	// UE_LOG(LogTemp, Warning, TEXT("Obey %d"), CanObeyOrder);
	
	return URTSOrderHelper::CheckOrder(Pawn, OrderData).IsEmpty();

	// return URTSOrderHelper::CanObeyOrder(OrderType, Pawn, OrderIndex);
}

void URTSBTDecorator_CanObeyOrderOnTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);

	const bool bResult = CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
	
	// // if (!CalculateRawConditionValue(OwnerComp, NodeMemory))
	// {
	// 	bool bShouldAbort = false;
	// 	const bool bIsOnActiveBranch = OwnerComp.IsExecutingBranch(GetMyNode(), GetChildIndex());
	// 	if (bIsOnActiveBranch)
	// 	{
	// 		bShouldAbort = (FlowAbortMode == EBTFlowAbortMode::Self || FlowAbortMode == EBTFlowAbortMode::Both) && CalculateRawConditionValue(OwnerComp, NodeMemory) == IsInversed();
	// 	}
	// 	else
	// 	{
	// 		bShouldAbort = (FlowAbortMode == EBTFlowAbortMode::LowerPriority || FlowAbortMode == EBTFlowAbortMode::Both) && CalculateRawConditionValue(OwnerComp, NodeMemory) != IsInversed();
	// 	}
	// 	
	// 	
	// 	if (bShouldAbort)
	// 	{
	// 		OwnerComp.RequestExecution(this);
	// 	}
	// }
}

uint16 URTSBTDecorator_CanObeyOrderOnTarget::GetInstanceMemorySize() const
{
	return sizeof(TNodeInstanceMemory);
}
