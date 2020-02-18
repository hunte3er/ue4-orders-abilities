// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/RTSBTTask_ClearBlackboardValue.h"
#include "Abilities/GameplayAbility.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type URTSBTTask_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard->IsValidKey(BlackboardKey.GetSelectedKeyID()))
	{
		Blackboard->ClearValue(BlackboardKey.GetSelectedKeyID());
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
