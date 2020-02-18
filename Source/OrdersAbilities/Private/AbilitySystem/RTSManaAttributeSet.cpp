// #include "AbilitySystem/RTSManaAttributeSet.h"
//
// #include "AbilitySystemComponent.h"
// #include "GameplayTagContainer.h"
// #include "Net/UnrealNetwork.h"
//
// #include "AbilitySystem/RTSGlobalTags.h"
//
// void URTSManaAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
// {
//     Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
// 	DOREPLIFETIME_CONDITION_NOTIFY(URTSManaAttributeSet, Mana, COND_None, REPNOTIFY_Always);
// 	DOREPLIFETIME_CONDITION_NOTIFY(URTSManaAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
// 	DOREPLIFETIME_CONDITION_NOTIFY(URTSManaAttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
// }
//
// URTSManaAttributeSet::URTSManaAttributeSet()
// {
// 	Mana = MaxMana = 100.0f;
// 	ManaRegen = 2.0f;
// }
//
// void URTSManaAttributeSet::Init()
// {
// 	//BIND_ATTRIBUTE_MAXVALUE_CHANGED(URTSManaAttributeSet, MaxMana);
// }
//
// void URTSManaAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
// {
// 	Super::PreAttributeBaseChange(Attribute, NewValue);
//
// 	if (Attribute == GetManaAttribute())
//     {
//         NewValue = FMath::Max(NewValue, 0.0f);
//     }
// }
