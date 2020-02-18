#pragma once

#include "CoreMinimal.h"
#include "Orders/RTSOrderWithBehavior.h"
#include "Orders/RTSTargetType.h"
#include "RTSCharacterAIOrder.generated.h"

class AActor;
class UBehaviorTree;

/**
 * Represents an order type that is meant to be issued to pawn that are controlled by the 'UOrdersAbilitiesAIController'.
 */
UCLASS(BlueprintType, Abstract, Blueprintable)
class ORDERSABILITIES_API URTSCharacterAIOrder : public URTSOrderWithBehavior
{
    GENERATED_BODY()

public:
    URTSCharacterAIOrder();

    //~ Begin URTSOrder Interface
    virtual bool IsTargetTypeFlagChecked(const AActor* OrderedActor, int32 Index, int32 InFlag) const override;
    virtual bool IsTargetTypeFlagChecked(const AActor* OrderedActor, int32 Index, ERTSTargetTypeFlags InFlag) const override;
    virtual bool IsCreatingIndividualTargetLocations(const AActor* OrderedActor, int32 Index) const override;
    virtual ERTSOrderGroupExecutionType GetGroupExecutionType(const AActor* OrderedActor, int32 Index) const override;
    //~ End URTSOrder Interface

protected:
    UPROPERTY(Category = RTS, EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = ERTSTargetTypeFlags, AllowPrivateAccess = true))
    int32 TargetTypeFlags;

    /**
     * To how many and which of the selected units should this order be issued to.
     */
    UPROPERTY(Category = RTS, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    ERTSOrderGroupExecutionType GroupExecutionType;

    /** Whether this order type creates individual target locations for a group of actors */
    UPROPERTY(Category = RTS, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
    bool bIsCreatingIndividualTargetLocations;
};
