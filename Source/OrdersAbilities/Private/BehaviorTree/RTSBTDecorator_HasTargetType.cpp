// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/RTSBTDecorator_HasTargetType.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Orders/RTSBlackboardHelper.h"
#include "AbilitySystem/RTSAbilitySystemComponent.h"
#include "Orders/RTSOrderComponent.h"

bool URTSBTDecorator_HasTargetType::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	URTSOrderComponent* OrderComponent = Pawn->FindComponentByClass<URTSOrderComponent>();

	TSoftClassPtr<URTSOrder> OrderType = OrderComponent->GetCurrentOrderType();
	
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	// auto OrderType = Cast<URTSOrder>(Blackboard->GetValueAsClass(URTSBlackboardHelper::BLACKBOARD_KEY_ORDER_TYPE));
	
	if (!OrderType.IsValid())
	{
		OrderType.LoadSynchronous();
	}

	int Index = Blackboard->GetValueAsInt(URTSBlackboardHelper::BLACKBOARD_KEY_INDEX);
	
	return OrderType->GetDefaultObject<URTSOrder>()->IsTargetTypeFlagChecked(Pawn, Index, TargetTypeFlags);
}
