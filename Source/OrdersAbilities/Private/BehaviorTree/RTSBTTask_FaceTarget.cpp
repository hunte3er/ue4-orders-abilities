// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/RTSBTTask_FaceTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Orders/RTSBlackboardHelper.h"
#include "Orders/RTSOrderHelper.h"
#include "Kismet/KismetMathLibrary.h"

URTSBTTask_FaceTarget::URTSBTTask_FaceTarget()
{
	NodeName = "Face Target";

	bNotifyTick = true;

	// UE_LOG(LogTemp, Warning, TEXT("Move to task!"));
}

EBTNodeResult::Type URTSBTTask_FaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// EBTNodeResult::Type NodeResult = EBTNodeResult::InProgress;
	// const FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(URTSBlackboardHelper::BLACKBOARD_KEY_LOCATION);
	// FAIMoveRequest MoveRequest(TargetLocation);
	//
	// OwnerComp.GetAIOwner()->MoveToActor();

	// UE_LOG(LogTemp, Warning, TEXT("Execute Task!"));

	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	
	return IsFacingTarget(Pawn, GetTargetLocation(Blackboard))
		? EBTNodeResult::Succeeded
		: EBTNodeResult::InProgress;
}

void URTSBTTask_FaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	FVector TargetLocation = GetTargetLocation(Blackboard);

	if (IsFacingTarget(Pawn, TargetLocation))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		// @TODO add a turn rate attribute
		// float TurnRate;

		FVector PawnLocation = Pawn->GetActorLocation();
		TargetLocation.Z = PawnLocation.Z;
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(PawnLocation, TargetLocation);
		const FRotator CurrentRotation = Pawn->GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, 8.0f);
		Pawn->SetActorRotation(NewRotation);
	}
}

FVector URTSBTTask_FaceTarget::GetTargetLocation(UBlackboardComponent* Blackboard)
{
	if (AActor* TargetAsActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName)))
	{
		return TargetAsActor->GetActorLocation();
	}
	else
	{
		return Blackboard->GetValueAsVector(TargetKey.SelectedKeyName);
	}
}

bool URTSBTTask_FaceTarget::IsFacingTarget(APawn* Pawn, const FVector& TargetLocation)
{
	const FVector ToTarget = TargetLocation - Pawn->GetActorLocation();

	float DotProduct = FVector::DotProduct(Pawn->GetActorForwardVector(), ToTarget.GetSafeNormal2D());

	return DotProduct >= 0.9;
}
