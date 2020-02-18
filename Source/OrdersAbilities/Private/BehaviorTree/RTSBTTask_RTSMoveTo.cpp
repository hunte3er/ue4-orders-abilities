// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/RTSBTTask_RTSMoveTo.h"
#include "AIController.h"
#include "Orders/RTSBlackboardHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Orders/RTSOrderHelper.h"
#include "Components/CapsuleComponent.h"

URTSBTTask_RTSMoveTo::URTSBTTask_RTSMoveTo()
{
	NodeName = "RTS Move To";

	bNotifyTick = true;
}

EBTNodeResult::Type URTSBTTask_RTSMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	AActor* TargetAsActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	const FVector TargetLocation = GetTargetLocation(Blackboard, TargetAsActor);
	
	return IsInRange(Pawn, Blackboard, TargetLocation, TargetAsActor)
		? EBTNodeResult::Succeeded
		: EBTNodeResult::InProgress;
}

void URTSBTTask_RTSMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	AActor* TargetAsActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	FVector TargetLocation = GetTargetLocation(Blackboard, TargetAsActor);

	// const FVector ToLocation = TargetLocation - Pawn->GetActorLocation();
	//
	// float Distance = ToLocation.Size2D();
	//
	// UCapsuleComponent* MyCapsule = Cast<UCapsuleComponent>(Pawn->GetComponentByClass(UCapsuleComponent::StaticClass()));
	// if (MyCapsule)
	// {
	// 	Distance -= MyCapsule->GetScaledCapsuleRadius();
	// }
	//
	// if (TargetAsActor != nullptr)
	// {
	// 	UCapsuleComponent* OtherCapsule = Cast<UCapsuleComponent>(TargetAsActor->GetComponentByClass(UCapsuleComponent::StaticClass()));
	// 	if (OtherCapsule)
	// 	{
	// 		Distance -= OtherCapsule->GetScaledCapsuleRadius();
	// 	}
	// }
	//
	// float OrderRange = URTSBlackboardHelper::GetBlackboardOrderRange(Blackboard);

	// if (Distance <= OrderRange)
	if (IsInRange(Pawn, Blackboard, TargetLocation, TargetAsActor))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		const FVector ToLocation = TargetLocation - Pawn->GetActorLocation();
		Pawn->AddMovementInput(ToLocation);
	}
}

FVector URTSBTTask_RTSMoveTo::GetTargetLocation(UBlackboardComponent* Blackboard, AActor* TargetAsActor)
{
	if (TargetAsActor != nullptr)
	{
		return TargetAsActor->GetActorLocation();
	}

	return Blackboard->GetValueAsVector(TargetKey.SelectedKeyName);
}

bool URTSBTTask_RTSMoveTo::IsInRange(APawn* Pawn, UBlackboardComponent* Blackboard, const FVector& TargetLocation, AActor* TargetAsActor)
{
	const FVector ToLocation = TargetLocation - Pawn->GetActorLocation();

	float Distance = ToLocation.Size2D();

	UCapsuleComponent* MyCapsule = Cast<UCapsuleComponent>(Pawn->GetComponentByClass(UCapsuleComponent::StaticClass()));
	if (MyCapsule)
	{
		Distance -= MyCapsule->GetScaledCapsuleRadius();
	}

	if (TargetAsActor != nullptr)
	{
		UCapsuleComponent* OtherCapsule = Cast<UCapsuleComponent>(TargetAsActor->GetComponentByClass(UCapsuleComponent::StaticClass()));
		if (OtherCapsule)
		{
			Distance -= OtherCapsule->GetScaledCapsuleRadius();
		}
	}

	float OrderRange = OverrideOrderRange >= 0.0f ? OverrideOrderRange : URTSBlackboardHelper::GetBlackboardOrderRange(Blackboard);

	return Distance <= OrderRange;
}
