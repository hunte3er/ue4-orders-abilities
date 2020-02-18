// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "Orders/RTSOrder.h"
#include "RTSBTService_AcquireTargets.generated.h"

/**
 * 
 */
UCLASS()
class ORDERSABILITIES_API URTSBTService_AcquireTargets : public UBTService
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = Decorator, EditAnywhere)
	TSoftClassPtr<URTSOrder> OrderType;

	UPROPERTY(Category = Decorator, EditAnywhere)
	int OrderIndex;

	UPROPERTY(Category = Decorator, EditAnywhere)
	bool bUseLocation = false;

	UPROPERTY(Category = Decorator, EditAnywhere)
	float Radius = 1000.0f;
	
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	void AcquireTargets(UBehaviorTreeComponent& OwnerComp);
	
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
};
