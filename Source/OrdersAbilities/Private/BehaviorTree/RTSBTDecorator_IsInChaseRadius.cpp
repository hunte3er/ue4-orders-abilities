// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/RTSBTDecorator_IsInChaseRadius.h"
#include "AIController.h"
#include "Orders/RTSOrderComponent.h"
#include "Orders/RTSOrderHelper.h"
#include "Orders/RTSBlackboardHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Orders/OrdersAbilitiesAIController.h"

URTSBTDecorator_IsInChaseRadius::URTSBTDecorator_IsInChaseRadius(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

bool URTSBTDecorator_IsInChaseRadius::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	const FVector HomeLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(URTSBlackboardHelper::BLACKBOARD_KEY_HOME_LOCATION);
	const float Dist = (HomeLocation - Pawn->GetActorLocation()).Size2D();
	// UE_LOG(LogTemp, Warning, TEXT("Dist %f"), Dist);
	
	return Dist < ChaseDistance; // @TODO Add chase range attribute?
}

void URTSBTDecorator_IsInChaseRadius::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!CalculateRawConditionValue(OwnerComp, NodeMemory))
	{
		OwnerComp.RequestExecution(this);
	}
}
