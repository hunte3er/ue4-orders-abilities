#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ERTSTargetTypeFlags : uint8
{
    /** No specific target (AOE, Aura or automatically applied to self). */
    NONE = 0,

    /** Order needs an actor as target. */
    ACTOR = 1 << 0,

    /** Order needs vector location as target. */
    LOCATION = 1 << 1,

    /** Order needs vector location that is used together with the unit location as a direction. */
    DIRECTION = 1 << 2,

    /** Can't be activated. */
    PASSIVE = 1 << 3
};
ENUM_CLASS_FLAGS(ERTSTargetTypeFlags)