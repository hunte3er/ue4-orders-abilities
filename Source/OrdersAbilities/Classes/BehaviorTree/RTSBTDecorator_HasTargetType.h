// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "Orders/RTSTargetType.h"
#include "RTSBTDecorator_HasTargetType.generated.h"


/**
 * 
 */
UCLASS()
class ORDERSABILITIES_API URTSBTDecorator_HasTargetType : public UBTDecorator
{
	GENERATED_BODY()

	UPROPERTY(Category = Decorator, EditAnywhere, meta = (Bitmask, BitmaskEnum = ERTSTargetTypeFlags))
	int32 TargetTypeFlags;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
