#include "Orders/RTSOrderWithBehavior.h"

#include "OrdersAbilities/OrdersAbilities.h"

#include "Math/Vector2D.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

#include "RTSAIController.h"
#include "Orders/RTSOrder.h"
#include "Orders/RTSOrderData.h"
#include "Orders/RTSOrderResult.h"
#include "Orders/RTSOrderTargetData.h"


URTSOrderWithBehavior::URTSOrderWithBehavior()
{
    bShouldRestartBehaviourTree = true;
    AcquisitionRadiusOverride = 0.0f;
    bIsAcquisitionRadiusOverridden = false;
}

UBehaviorTree* URTSOrderWithBehavior::GetBehaviorTree() const
{
    return BehaviorTree;
}

bool URTSOrderWithBehavior::ShouldRestartBehaviourTree() const
{
    return bShouldRestartBehaviourTree;
}

void URTSOrderWithBehavior::IssueOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index,
                                       FRTSOrderCallback Callback, const FVector& HomeLocation) const
{
    if (!IsValid(OrderedActor))
    {
        UE_LOG(LogRTS, Error, TEXT("Ordered actor is invalid."));
        Callback.Broadcast(ERTSOrderResult::FAILED);
        return;
    }

    APawn* Pawn = Cast<APawn>(OrderedActor);
    if (Pawn == nullptr)
    {
        UE_LOG(LogRTS, Error, TEXT("The specified actor '%s' is not a pawn."), *OrderedActor->GetName());
        Callback.Broadcast(ERTSOrderResult::FAILED);
        return;
    }

    ARTSAIController* Controller = Cast<ARTSAIController>(Pawn->GetController());
    if (Controller == nullptr)
    {
        UE_LOG(
            LogRTS, Error,
            TEXT(
                "The specified pawn '%s' does not have the required 'RTSAIController' to receive RTS orders."),
            *OrderedActor->GetName());
        Callback.Broadcast(ERTSOrderResult::FAILED);
        return;
    }

    AActor* TargetActor = TargetData.Actor;
    const FVector2D TargetLocation = TargetData.Location;

    FRTSOrderData Order(GetClass(), Index, TargetActor, TargetLocation);

	Order.bUseLocation = IsTargetTypeFlagChecked(OrderedActor, Index, ERTSTargetTypeFlags::LOCATION | ERTSTargetTypeFlags::DIRECTION);

    Controller->IssueOrder(Order, Callback, HomeLocation);
}

bool URTSOrderWithBehavior::GetAcquisitionRadiusOverride(const AActor* OrderedActor, int32 Index,
                                                         float& OutAcquisitionRadius) const
{
    OutAcquisitionRadius = AcquisitionRadiusOverride;
    return bIsAcquisitionRadiusOverridden;
}
