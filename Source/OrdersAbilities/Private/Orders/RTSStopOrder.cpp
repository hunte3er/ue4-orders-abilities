#include "Orders/RTSStopOrder.h"

#include "GameFramework/Actor.h"

#include "AbilitySystem/RTSGlobalTags.h"
#include "RTSOwnerComponent.h"
#include "AbilitySystem/RTSAbilitySystemComponent.h"


URTSStopOrder::URTSStopOrder()
{
    TargetTypeFlags = static_cast<int32>(ERTSTargetTypeFlags::NONE);
    bIsCreatingIndividualTargetLocations = false;

    TagRequirements.SourceBlockedTags.AddTag(URTSGlobalTags::Status_Changing_Constructing());
}

bool URTSStopOrder::AreAutoOrdersAllowedDuringOrder() const
{
    return true;
}