#include "Orders/RTSCharacterAIOrder.h"

#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Actor.h"


URTSCharacterAIOrder::URTSCharacterAIOrder()
{
    TargetTypeFlags = static_cast<int32>(ERTSTargetTypeFlags::NONE);
	
    bIsCreatingIndividualTargetLocations = false;

    GroupExecutionType = ERTSOrderGroupExecutionType::ALL;
}

bool URTSCharacterAIOrder::IsTargetTypeFlagChecked(const AActor* OrderedActor, int32 Index, int32 InFlag) const
{
    return (TargetTypeFlags & InFlag) != 0;
}

bool URTSCharacterAIOrder::IsCreatingIndividualTargetLocations(const AActor* OrderedActor, int32 Index) const
{
    return bIsCreatingIndividualTargetLocations;
}

ERTSOrderGroupExecutionType URTSCharacterAIOrder::GetGroupExecutionType(const AActor* OrderedActor, int32 Index) const
{
    return GroupExecutionType;
}
