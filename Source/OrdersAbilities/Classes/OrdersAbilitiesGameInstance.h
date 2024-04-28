#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OrdersAbilitiesGameInstance.generated.h"

UCLASS()
class ORDERSABILITIES_API UOrdersAbilitiesGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init() override;
};
