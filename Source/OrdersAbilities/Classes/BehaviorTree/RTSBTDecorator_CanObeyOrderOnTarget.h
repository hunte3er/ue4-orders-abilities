// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Orders/RTSOrder.h"
#include "RTSBTDecorator_CanObeyOrderOnTarget.generated.h"

struct FBTObeyOrderNodeMemory
{
	bool bLastRawResult;
};

UCLASS()
class ORDERSABILITIES_API URTSBTDecorator_CanObeyOrderOnTarget : public UBTDecorator
{
	GENERATED_UCLASS_BODY()

	typedef FBTObeyOrderNodeMemory TNodeInstanceMemory;

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<URTSOrder> OrderType;
	
	UPROPERTY(EditAnywhere)
	int OrderIndex = -1;

	UPROPERTY(EditAnywhere)
	bool bUseLocation = false;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
};
