#include "Orders/RTSOrder.h"

#include "AbilitySystem/RTSGlobalTags.h"
#include "Orders/RTSOrderProcessPolicy.h"
#include "RTSUtilities.h"


URTSOrder::URTSOrder()
{
    OrderProcessPolicy = ERTSOrderProcessPolicy::CAN_BE_CANCELED;
    TagRequirements.SourceRequiredTags.AddTag(URTSGlobalTags::Status_Changing_IsAlive());
}

bool URTSOrder::CanObeyOrder(const AActor* OrderedActor, int32 Index,
                             FRTSOrderErrorTags* OutErrorTags /*= nullptr*/) const
{
    return true;
}

bool URTSOrder::CanObeyOrder(const AActor* OrderedActor, const FRTSOrderData& OrderData, FRTSOrderErrorTags* OutErrorTags) const
{
    return true;
}

bool URTSOrder::IsValidTarget(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index,
                              FRTSOrderErrorTags* OutErrorTags /*= nullptr*/) const
{
    return true;
}

void URTSOrder::CreateIndividualTargetLocations(const TArray<AActor*>& OrderedActors,
                                                const FRTSOrderTargetData& TargetData,
                                                TArray<FVector2D>& OutTargetLocations) const
{
    for (int32 i = 0; i < OrderedActors.Num(); ++i)
    {
        OutTargetLocations.Add(TargetData.Location);
    }
}

void URTSOrder::IssueOrder(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index,
                           FRTSOrderCallback Callback, const FVector& HomeLocation) const
{
    check(0);
}

void URTSOrder::OrderCanceled(AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
}

bool URTSOrder::IsTargetTypeFlagChecked(const AActor* OrderedActor, int32 Index, int32 InFlag) const
{
    return false;
}

bool URTSOrder::IsTargetTypeFlagChecked(const AActor* OrderedActor, int32 Index, ERTSTargetTypeFlags InFlag) const
{
    return false;
}

bool URTSOrder::IsCreatingIndividualTargetLocations(const AActor* OrderedActor, int32 Index) const
{
    return false;
}

UTexture2D* URTSOrder::GetNormalIcon(const AActor* OrderedActor, int32 Index) const
{
    return nullptr;
}

UTexture2D* URTSOrder::GetHoveredIcon(const AActor* OrderedActor, int32 Index) const
{
    return nullptr;
}

UTexture2D* URTSOrder::GetPressedIcon(const AActor* OrderedActor, int32 Index) const
{
    return nullptr;
}

UTexture2D* URTSOrder::GetDisabledIcon(const AActor* OrderedActor, int32 Index) const
{
    return nullptr;
}

FText URTSOrder::GetName(const AActor* OrderedActor, int32 Index) const
{
    return FText::FromString(GetClass()->GetName());
}

FText URTSOrder::GetDescription(const AActor* OrderedActor, int32 Index) const
{
    return FText();
}

int32 URTSOrder::GetOrderButtonIndex() const
{
    return -1;
}

bool URTSOrder::HasFixedOrderButtonIndex() const
{
    return false;
}

FRTSOrderPreviewData URTSOrder::GetOrderPreviewData(const AActor* OrderedActor, int32 Index) const
{
    return OrderPreviewData;
}

void URTSOrder::GetTagRequirements(const AActor* OrderedActor, int32 Index,
                                   FRTSOrderTagRequirements& OutTagRequirements) const
{
    OutTagRequirements = TagRequirements;
}

void URTSOrder::GetSuccessTagRequirements(const AActor* OrderedActor, int32 Index,
                                          FRTSOrderTagRequirements& OutTagRequirements) const
{
    OutTagRequirements = SuccessTagRequirements;
}

float URTSOrder::GetRequiredRange(const AActor* OrderedActor, int32 Index) const
{
    return 0;
}

bool URTSOrder::GetAcquisitionRadiusOverride(const AActor* OrderedActor, int32 Index, float& OutAcquisitionRadius) const
{
    return false;
}

ERTSOrderProcessPolicy URTSOrder::GetOrderProcessPolicy(const AActor* OrderedActor, int32 Index) const
{
    return OrderProcessPolicy;
}

TSoftClassPtr<URTSOrder> URTSOrder::GetFallbackOrder() const
{
    return FallbackOrder;
}

float URTSOrder::GetTargetScore(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index) const
{
    // TODO: Implement this function individually for each order type instead of here in the base class.
    if (!IsValid(OrderedActor))
    {
        return 0.0f;
    }

    float Distance = 0.0f;

    if (IsValid(TargetData.Actor))
    {
        // Subtract the half collision size of the target from the distance.
        Distance = FVector::Dist2D(OrderedActor->GetActorLocation(), TargetData.Actor->GetActorLocation());

		// TODO: This is only relevant for melee units and should be ignored for ranged units.
		bool bIsMeleeUnit = true;
		if (bIsMeleeUnit)
		{
			Distance -= URTSUtilities::GetActorCollisionSize(TargetData.Actor) / 2.0f;
		}
    }

    else
    {
        Distance = FVector::Dist2D(OrderedActor->GetActorLocation(), FVector(TargetData.Location, 0.0f));
    }

    float AcquisitionRadius;
    if (!GetAcquisitionRadiusOverride(OrderedActor, Index, AcquisitionRadius))
    {
        // NOTE(np): In A Year Of Rain, units have a specific radius in which to automatically acquire targets.
        /*const URTSAttackComponent* AttackComponent = OrderedActor->FindComponentByClass<URTSAttackComponent>();
        if (AttackComponent == nullptr)
        {
            return 0.0f;
        }

        AcquisitionRadius = AttackComponent->GetAcquisitionRadius();*/
        AcquisitionRadius = 100000.0f;
    }

    return 1.0f - Distance / AcquisitionRadius;
}

ERTSOrderGroupExecutionType URTSOrder::GetGroupExecutionType(const AActor* OrderedActor, int32 Index) const
{
    return ERTSOrderGroupExecutionType::ALL;
}

bool URTSOrder::IsHumanPlayerAutoOrder(const AActor* OrderedActor, int32 Index) const
{
    return false;
}

bool URTSOrder::GetHumanPlayerAutoOrderInitialState(const AActor* OrderedActor, int32 Index) const
{
    return false;
}

bool URTSOrder::IsAIPlayerAutoOrder(const AActor* OrderedActor, int32 Index) const
{
    return false;
}

bool URTSOrder::AreAutoOrdersAllowedDuringOrder() const
{
    return false;
}
