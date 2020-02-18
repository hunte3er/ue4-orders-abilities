#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "RTSOrderTargetData.generated.h"

class AActor;

/** Targeting data for an order. */
USTRUCT(BlueprintType)
struct ORDERSABILITIES_API FRTSOrderTargetData
{
    GENERATED_BODY()

    FRTSOrderTargetData();
    FRTSOrderTargetData(AActor* InActor, const FVector2D InLocation, const FGameplayTagContainer& InTargetTags = FGameplayTagContainer::EmptyContainer);

    /** The target actor. */
    UPROPERTY(Category = RTS, EditDefaultsOnly, BlueprintReadWrite)
    AActor* Actor;

    /** The target location. */
    UPROPERTY(Category = RTS, EditDefaultsOnly, BlueprintReadWrite)
    FVector2D Location;

    /** Indicates that 'Location' is valid and should be used by this order. */
    UPROPERTY(Category = RTS, EditAnywhere, BlueprintReadWrite, meta = (InlineEditConditionToggle = true))
	uint8 bUseLocation : 1;

    /** The target tags. */
    UPROPERTY(Category = RTS, EditDefaultsOnly, BlueprintReadWrite)
    FGameplayTagContainer TargetTags;

    /**
     * Get a textual representation of this order target data.
     * @return A string describing the order target data.
     */
    FString ToString() const;
};
