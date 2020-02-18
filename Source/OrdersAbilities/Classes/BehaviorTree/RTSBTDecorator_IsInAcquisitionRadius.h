// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "RTSBTDecorator_IsInAcquisitionRadius.generated.h"

struct FBTIsInAcquisitionRadiusNodeMemory
{
	bool bLastRawResult;
};

UCLASS()
class ORDERSABILITIES_API URTSBTDecorator_IsInAcquisitionRadius : public UBTDecorator
{
	GENERATED_UCLASS_BODY()

	typedef FBTIsInAcquisitionRadiusNodeMemory TNodeInstanceMemory;
	
	UPROPERTY(EditAnywhere)
	float Radius = 800.0f;
	
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override;
};
