#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RTSGameInstance.generated.h"

UCLASS()
class ORDERSABILITIES_API URTSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void Init() override;
};
