#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BTNode.h"
#include "Orders/RTSOrder.h"
#include "RTSBlackboardHelper.generated.h"

/** Helper function for the behavior trees. */
UCLASS(meta = (RestrictedToClasses = "BTNode"))
class ORDERSABILITIES_API URTSBlackboardHelper : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, Category = "RTS|BehaviorTree", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
    static TSubclassOf<URTSOrder> GetBlackboardOrderType(UBlackboardComponent* BlackboardComp);

    UFUNCTION(BlueprintPure, Category = "RTS|BehaviorTree", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
    static FVector GetBlackboardOrderLocation(UBlackboardComponent* BlackboardComp);

    UFUNCTION(BlueprintPure, Category = "RTS|BehaviorTree", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
    static AActor* GetBlackboardOrderTarget(UBlackboardComponent* BlackboardComp);

    UFUNCTION(BlueprintPure, Category = "RTS|BehaviorTree", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
    static int32 GetBlackboardOrderIndex(UBlackboardComponent* BlackboardComp);

    UFUNCTION(BlueprintPure, Category = "RTS|BehaviorTree", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
    static float GetBlackboardOrderRange(UBlackboardComponent* BlackboardComp);

    UFUNCTION(BlueprintPure, Category = "RTS|BehaviorTree", Meta = (HidePin = "NodeOwner", DefaultToSelf = "NodeOwner"))
    static FVector GetBlackboardOrderHomeLocation(UBlackboardComponent* BlackboardComp);

    static const FName BLACKBOARD_KEY_ORDER_TYPE;
    static const FName BLACKBOARD_KEY_LOCATION;
    static const FName BLACKBOARD_KEY_TARGET;
    static const FName BLACKBOARD_KEY_INDEX;
    static const FName BLACKBOARD_KEY_RANGE;
    static const FName BLACKBOARD_KEY_HOME_LOCATION;
};