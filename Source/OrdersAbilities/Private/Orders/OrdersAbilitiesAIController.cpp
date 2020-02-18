#include "Orders/OrdersAbilitiesAIController.h"

#include "OrdersAbilities/OrdersAbilities.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Controller.h"

#include "Orders/RTSBlackboardHelper.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderHelper.h"
#include "Orders/RTSOrderWithBehavior.h"
#include "Orders/RTSStopOrder.h"
#include "Navigation/CrowdFollowingComponent.h"


AOrdersAbilitiesAIController::AOrdersAbilitiesAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
    PrimaryActorTick.bCanEverTick = true;

    UCrowdFollowingComponent* CrowdFollowing = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent());
	if (CrowdFollowing)
	{
		CrowdFollowing->SetCrowdSlowdownAtGoal(false, false);
		CrowdFollowing->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium, false);
		CrowdFollowing->SetCrowdRotateToVelocity(false);
		//CrowdFollowing->SetCrowdPathOffset(true, false);
		CrowdFollowing->SetCrowdSeparation(true, false);
		CrowdFollowing->UpdateCrowdAgentParams();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Crowd following invalid"));
	}
}

void AOrdersAbilitiesAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Load assets.
    StopOrder.LoadSynchronous();

    // Make AI use assigned blackboard.
    UBlackboardComponent* BlackboardComponent;

    if (UseBlackboard(CharacterBlackboardAsset, BlackboardComponent))
    {
        // Setup blackboard.
        SetBlackboardValues(FRTSOrderData(StopOrder.Get()), InPawn->GetActorLocation());
    }

	// fpwong: we shouldn't need this anymore as we call issue order in the RTSAbilitySystemComponent
    // // Call RunBehaviorTree. This will setup the behavior tree component.
    UBehaviorTree* BehaviorTree = URTSOrderHelper::GetBehaviorTree(StopOrder.Get());
    RunBehaviorTree(BehaviorTree);
}

TSubclassOf<AActor> AOrdersAbilitiesAIController::GetBuildingClass() const
{
    if (!VerifyBlackboard())
    {
        return nullptr;
    }

    // NOTE(np): In A Year Of Rain, each builder unit has its own set of constructible buildings.
    /*URTSBuilderComponent* BuilderComponent = GetPawn()->FindComponentByClass<URTSBuilderComponent>();

    if (!BuilderComponent)
    {
        return nullptr;
    }

    int32 BuildingIndex = Blackboard->GetValueAsInt(URTSBlackboardHelper::BLACKBOARD_KEY_INDEX);

    return BuilderComponent->ConstructibleBuildingClasses.IsValidIndex(BuildingIndex)
               ? BuilderComponent->ConstructibleBuildingClasses[BuildingIndex]
               : nullptr;*/

    return nullptr;
}

bool AOrdersAbilitiesAIController::HasOrder(TSubclassOf<URTSOrder> OrderType) const
{
    if (!VerifyBlackboard())
    {
        return false;
    }

    return Blackboard->GetValueAsClass(URTSBlackboardHelper::BLACKBOARD_KEY_ORDER_TYPE) == OrderType;
}

void AOrdersAbilitiesAIController::IssueOrder(const FRTSOrderData& Order, FRTSOrderCallback Callback,
                                           const FVector& HomeLocation)
{
    UBehaviorTree* BehaviorTree = URTSOrderHelper::GetBehaviorTree(Order.OrderType.Get());
    if (BehaviorTree == nullptr)
    {
        Callback.Broadcast(ERTSOrderResult::FAILED);
        return;
    }

    CurrentOrderResultCallback = Callback;
    BehaviorTreeResult = EBTNodeResult::InProgress;

    SetBlackboardValues(Order, HomeLocation);

    // Stop any current orders and start over.
    ApplyOrder(Order, BehaviorTree);
}

TSoftClassPtr<URTSStopOrder> AOrdersAbilitiesAIController::GetStopOrder() const
{
    return StopOrder;
}

void AOrdersAbilitiesAIController::BehaviorTreeEnded(EBTNodeResult::Type Result)
{
    if (!VerifyBlackboard())
    {
        return;
    }

    switch (Result)
    {
        case EBTNodeResult::InProgress:
            return;
        case EBTNodeResult::Failed:
            BehaviorTreeResult = EBTNodeResult::Failed;
            return;
        case EBTNodeResult::Aborted:
            return;
        case EBTNodeResult::Succeeded:
            BehaviorTreeResult = EBTNodeResult::Succeeded;
            return;
    }
}

FVector AOrdersAbilitiesAIController::GetHomeLocation()
{
    if (!VerifyBlackboard())
    {
        return FVector::ZeroVector;
    }

    return Blackboard->GetValueAsVector(URTSBlackboardHelper::BLACKBOARD_KEY_HOME_LOCATION);
}

void AOrdersAbilitiesAIController::SetBlackboardValues(const FRTSOrderData& Order, const FVector& HomeLocation)
{
    if (!VerifyBlackboard())
    {
        return;
    }

    Blackboard->SetValueAsClass(URTSBlackboardHelper::BLACKBOARD_KEY_ORDER_TYPE, Order.OrderType.Get());
    if (Order.bUseLocation)
    {
        // NOTE(np): In A Year Of Rain, we're using a raycast to translate between 3D and 2D space.
        /*Blackboard->SetValueAsVector(URTSBlackboardHelper::BLACKBOARD_KEY_LOCATION,
                                     URTSUtilities::GetGroundLocation2D(this, Order.Location));*/
        Blackboard->SetValueAsVector(URTSBlackboardHelper::BLACKBOARD_KEY_LOCATION,
            FVector(Order.Location.X, Order.Location.Y, 0.0f));
    }
    else
    {
        Blackboard->ClearValue(URTSBlackboardHelper::BLACKBOARD_KEY_LOCATION);
    }

    Blackboard->SetValueAsObject(URTSBlackboardHelper::BLACKBOARD_KEY_TARGET, Order.Target);
    Blackboard->SetValueAsInt(URTSBlackboardHelper::BLACKBOARD_KEY_INDEX, Order.Index);
    Blackboard->SetValueAsFloat(URTSBlackboardHelper::BLACKBOARD_KEY_RANGE,
                                URTSOrderHelper::GetOrderRequiredRange(Order.OrderType, GetPawn(), Order.Index));
    Blackboard->SetValueAsVector(URTSBlackboardHelper::BLACKBOARD_KEY_HOME_LOCATION, HomeLocation);
}

void AOrdersAbilitiesAIController::ApplyOrder(const FRTSOrderData& Order, UBehaviorTree* BehaviorTree)
{
    UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
    if (BehaviorTreeComponent != nullptr && BehaviorTree != nullptr)
    {
        // Make sure to really restart the tree if the same same tree that is currently executing is passed in.
        UBehaviorTree* CurrentTree = BehaviorTreeComponent->GetRootTree();
        if (CurrentTree == BehaviorTree)
        {
            if (URTSOrderHelper::ShouldRestartBehaviourTree(Order.OrderType.Get()))
            {
                BehaviorTreeComponent->RestartTree();
            }
        }
        else
        {
            // BehaviorTreeComponent->StartTree(*BehaviorTree, EBTExecutionMode::Looped);
            BehaviorTreeComponent->StartTree(*BehaviorTree, EBTExecutionMode::SingleRun);
        }
    }
}

bool AOrdersAbilitiesAIController::VerifyBlackboard() const
{
    if (!Blackboard)
    {
        UE_LOG(
            LogRTS, Warning,
            TEXT("Blackboard not set up for %s, can't receive orders. Check AI Controller Class and Auto Possess AI."),
            *GetPawn()->GetName());
        return false;
    }

    return true;
}

void AOrdersAbilitiesAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Blackboard == nullptr)
    {
        return;
    }

    UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (BehaviorTreeComponent)
	{
        if (!BehaviorTreeComponent->IsRunning())
        {
            CurrentOrderResultCallback.Broadcast(ERTSOrderResult::SUCCEEDED);
        }
	}

    // switch (BehaviorTreeResult)
    // {
    //     case EBTNodeResult::InProgress:
    //         break;
    //     case EBTNodeResult::Failed:
    //         CurrentOrderResultCallback.Broadcast(ERTSOrderResult::FAILED);
    //         break;
    //     case EBTNodeResult::Aborted:
    //         break;
    //     case EBTNodeResult::Succeeded:
    //         CurrentOrderResultCallback.Broadcast(ERTSOrderResult::SUCCEEDED);
    //         break;
    // }
    //
    // BehaviorTreeResult = EBTNodeResult::InProgress;
}
