// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/RTSBTTask_UseAbility.h"
#include "AIController.h"
#include "Orders/RTSBlackboardHelper.h"
#include "AbilitySystem/RTSAbilitySystemComponent.h"
#include "AbilitySystem/RTSAbilitySystemHelper.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Orders/RTSOrderHelper.h"

EBTNodeResult::Type URTSBTTask_UseAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	MyOwnerComp = &OwnerComp;

	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn)
		return EBTNodeResult::Failed;

	MyAbilitySystem = Pawn->FindComponentByClass<URTSAbilitySystemComponent>();
	if (!MyAbilitySystem)
		return EBTNodeResult::Failed;
	
	const TArray<TSubclassOf<UGameplayAbility>>& Abilities = URTSAbilitySystemHelper::GetOrderAbilities(MyAbilitySystem);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	
	const int AbilityIndex = BB->GetValueAsInt(URTSBlackboardHelper::BLACKBOARD_KEY_INDEX);
	if (!Abilities.IsValidIndex(AbilityIndex))
		return EBTNodeResult::Failed;

	const TSubclassOf<UGameplayAbility> AbilityClass = Abilities[AbilityIndex];
	
	MyAbility = AbilityClass->GetDefaultObject<UGameplayAbility>();

	const FVector2D Location = FVector2D(BB->GetValueAsVector(URTSBlackboardHelper::BLACKBOARD_KEY_LOCATION));
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(URTSBlackboardHelper::BLACKBOARD_KEY_TARGET));
	
	const FRTSOrderTargetData TargetData = URTSOrderHelper::CreateOrderTargetData(Pawn, Target, Location);

	FGameplayEventData EventData;
	URTSAbilitySystemHelper::CreateGameplayEventData(Pawn, TargetData, MyAbility->GetClass(), EventData);

	MyAbilitySystem->OnGameplayAbilityEnded.AddDynamic(this, &URTSBTTask_UseAbility::OnAbilityEnded);
		
	// if (MyAbilitySystem->TryActivateAbilityByClass(AbilityClass))
	if (MyAbilitySystem->TryActivateAbilityByClassWithEventData(AbilityClass, EventData))
	{
		return EBTNodeResult::InProgress;
	}
	
	MyAbilitySystem->OnGameplayAbilityEnded.RemoveDynamic(this, &URTSBTTask_UseAbility::OnAbilityEnded);
	
	// int32 TriggeredAbilities = URTSAbilitySystemHelper::SendGameplayEvent(Pawn, EventData);
	// if (TriggeredAbilities > 0)
	// {
	// 	MyAbilitySystem->OnGameplayAbilityEnded.RemoveDynamic(this, &URTSBTTask_UseAbility::OnAbilityEnded);
	// 	MyAbilitySystem->OnGameplayAbilityEnded.AddDynamic(this, &URTSBTTask_UseAbility::OnAbilityEnded);
	// 	return EBTNodeResult::InProgress;
	// }
	
	return EBTNodeResult::Failed;
}

EBTNodeResult::Type URTSBTTask_UseAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (MyAbilitySystem != nullptr && MyAbility != nullptr)
	{
		MyAbilitySystem->CancelAbility(MyAbility);
	}
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void URTSBTTask_UseAbility::OnAbilityEnded(TSubclassOf<UGameplayAbility> Ability, FGameplayAbilitySpecHandle AbilitySpecHandle, bool bWasCancelled)
{
	if (MyAbilitySystem)
	{
		MyAbilitySystem->OnGameplayAbilityEnded.RemoveDynamic(this, &URTSBTTask_UseAbility::OnAbilityEnded);
	}
	
	if (MyOwnerComp)
	{
		FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);
	}
}
