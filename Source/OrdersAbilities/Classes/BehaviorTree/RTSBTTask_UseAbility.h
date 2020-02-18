// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/RTSAbilitySystemComponent.h"
#include "RTSBTTask_UseAbility.generated.h"

UCLASS()
class ORDERSABILITIES_API URTSBTTask_UseAbility : public UBTTaskNode
{
	GENERATED_BODY()

	UBehaviorTreeComponent* MyOwnerComp;
	URTSAbilitySystemComponent* MyAbilitySystem;
	UGameplayAbility* MyAbility;
	
public:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UFUNCTION()
	void OnAbilityEnded(TSubclassOf<UGameplayAbility> Ability, FGameplayAbilitySpecHandle AbilitySpecHandle, bool bWasCancelled);
};
