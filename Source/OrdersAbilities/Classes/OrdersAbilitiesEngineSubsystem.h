#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "OrdersAbilitiesEngineSubsystem.generated.h"

UCLASS()
class ORDERSABILITIES_API UOrdersAbilitiesEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
