#include "OrdersAbilitiesGameInstance.h"
#include "AbilitySystemGlobals.h"

void UOrdersAbilitiesGameInstance::Init()
{
	UGameInstance::Init();
	
	UAbilitySystemGlobals::Get().InitGlobalData();
}
