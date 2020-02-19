// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/RTSBTDecorator_IsInAcquisitionRadius.h"
#include "AIController.h"
#include "Orders/RTSOrderComponent.h"
#include "Orders/RTSOrderHelper.h"
#include "Orders/RTSBlackboardHelper.h"
#include "BehaviorTree/BlackboardComponent.h"

URTSBTDecorator_IsInAcquisitionRadius::URTSBTDecorator_IsInAcquisitionRadius(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void URTSBTDecorator_IsInAcquisitionRadius::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TNodeInstanceMemory* DecoratorMemory = reinterpret_cast<TNodeInstanceMemory*>(NodeMemory);
	DecoratorMemory->bLastRawResult = CalculateRawConditionValue(OwnerComp, NodeMemory);
}

bool URTSBTDecorator_IsInAcquisitionRadius::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(URTSBlackboardHelper::BLACKBOARD_KEY_TARGET));

	TArray<AActor*> ActorsInRange;

	TSubclassOf<URTSOrder> OrderType = URTSBlackboardHelper::GetBlackboardOrderType(Blackboard);
	int32 OrderIndex = URTSBlackboardHelper::GetBlackboardOrderIndex(Blackboard);
	float AcquisitionRadius = Radius;

	float AcquisitionRadiusOverride;
	if (URTSOrderHelper::GetAcquisitionRadiusOverride(OrderType.Get(), Pawn, OrderIndex, AcquisitionRadiusOverride))
	{
		AcquisitionRadius = AcquisitionRadiusOverride;
	}
	
	URTSOrderHelper::FindActors(Pawn->GetWorld(), AcquisitionRadius, Pawn->GetActorLocation(), ActorsInRange);
	
	return ActorsInRange.Contains(Target);
}


void URTSBTDecorator_IsInAcquisitionRadius::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	TNodeInstanceMemory* DecoratorMemory = CastInstanceNodeMemory<TNodeInstanceMemory>(NodeMemory);

	const bool bResult = CalculateRawConditionValue(OwnerComp, NodeMemory);
	if (bResult != DecoratorMemory->bLastRawResult)
	{
		DecoratorMemory->bLastRawResult = bResult;
		OwnerComp.RequestExecution(this);
	}
}

uint16 URTSBTDecorator_IsInAcquisitionRadius::GetInstanceMemorySize() const
{
	return sizeof(TNodeInstanceMemory);
}
