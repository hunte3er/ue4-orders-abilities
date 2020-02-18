#include "AbilitySystem/RTSAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


void URTSAttributeSet::Init()
{
}

void URTSAttributeSet::PostInitializeProperties(bool bInitialInit)
{
}

void URTSAttributeSet::GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const
{
}

void URTSAttributeSet::GrantAbilities() const
{
	auto OwningActor = GetOwningActor();
	
	for (TSubclassOf<UGameplayEffect> EffectClass : EffectsToApply)
	{
		UGameplayEffect* GameplayEffect = EffectClass->GetDefaultObject<UGameplayEffect>();
		FGameplayEffectContextHandle ContextHandle;

		auto SpecHandle = UAbilitySystemBlueprintLibrary::MakeSpecHandle(GameplayEffect, OwningActor, OwningActor);
		
		GetOwningAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());//GameplayEffect, 0.0f, FGameplayEffectContextHandle());
	}
	
	for (TSubclassOf<UGameplayAbility> AbilityClass : AbilitiesToGrant)
	{
		GetOwningAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(AbilityClass->GetDefaultObject<UGameplayAbility>()));
	}
}
