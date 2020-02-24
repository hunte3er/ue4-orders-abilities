#include "RTSGameInstance.h"
#include "AbilitySystemGlobals.h"

void URTSGameInstance::Init()
{
	UGameInstance::Init();
	
	UAbilitySystemGlobals::Get().InitGlobalData();
}
