// Fill out your copyright notice in the Description page of Project Settings.

#include "BehaviorTree/RTSBTService_AcquireTargets.h"
#include "AIController.h"
#include "Orders/RTSOrderComponent.h"
#include "Orders/RTSOrderHelper.h"
#include "Orders/RTSBlackboardHelper.h"
#include "AbilitySystem/RTSAbilitySystemComponent.h"
#include "Orders/RTSAttackOrder.h"
#include "BehaviorTree/BlackboardComponent.h"

void URTSBTService_AcquireTargets::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	AcquireTargets(OwnerComp);

	UE_LOG(LogTemp, Warning, TEXT("On become relevant?"));
	
	// APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	//
	// URTSOrderComponent* OrderComponent = Pawn->FindComponentByClass<URTSOrderComponent>();
	//
	// UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	//
	// //AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(URTSBlackboardHelper::BLACKBOARD_KEY_TARGET));
	//
	//
	// float Radius = 1000;
	// TArray<AActor*> ActorsInRange;
	// URTSOrderHelper::FindActors(Pawn->GetWorld(), Radius, Pawn->GetActorLocation(), ActorsInRange);
	//
	// if (ActorsInRange.Num() > 0)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Found target!"));
	// 	for (AActor* Actor : ActorsInRange)
	// 	{
	// 		if (Actor != Pawn)
	// 		{
	// 			Blackboard->SetValueAsObject(URTSBlackboardHelper::BLACKBOARD_KEY_TARGET, Actor);
	// 			break;
	// 		}
	// 	}
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("No target in range!"));
	// }
}

void URTSBTService_AcquireTargets::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AcquireTargets(OwnerComp);
}

void URTSBTService_AcquireTargets::AcquireTargets(UBehaviorTreeComponent& OwnerComp)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();

	URTSOrderComponent* OrderComponent = Pawn->FindComponentByClass<URTSOrderComponent>();

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	//AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(URTSBlackboardHelper::BLACKBOARD_KEY_TARGET));

	TArray<AActor*> ActorsInRange;
	URTSOrderHelper::FindActors(Pawn->GetWorld(), Radius, Pawn->GetActorLocation(), ActorsInRange);

	FVector MyLocation = Pawn->GetActorLocation();
	const auto ClosestSorter = [MyLocation](const AActor& A, const AActor& B)
	{
		return FVector::DistSquared2D(MyLocation, A.GetActorLocation()) < FVector::DistSquared2D(MyLocation, B.GetActorLocation());
	};
	
	ActorsInRange.Sort(ClosestSorter);


	bool bFoundActor = false;
	
	if (ActorsInRange.Num() > 0)
	{		
		for (AActor* Actor : ActorsInRange)
		{
			FRTSOrderData OrderData;
			OrderData.OrderType = OrderType;
			OrderData.Index = OrderIndex;
			OrderData.Target = Actor;
			OrderData.bUseLocation = bUseLocation;
			
			if (URTSOrderHelper::CheckOrder(Pawn, OrderData).IsEmpty())
			{
				// if (Actor != Pawn)
				{
					UE_LOG(LogTemp, Warning, TEXT("Found target for %s! %s"), *Pawn->GetName(), *Actor->GetName());
					bFoundActor = true;
					Blackboard->SetValueAsObject(URTSBlackboardHelper::BLACKBOARD_KEY_TARGET, Actor);
					break;
				}
			}
		}
	}
	
	if (!bFoundActor)
	{
		//Blackboard->SetValueAsObject(URTSBlackboardHelper::BLACKBOARD_KEY_TARGET, nullptr);
		// UE_LOG(LogTemp, Warning, TEXT("No target in range!"));
	}
}

void URTSBTService_AcquireTargets::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	
}
