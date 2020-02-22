#include "AbilitySystem/RTSProgressionAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystem/RTSGlobalTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/RTSAbilitySystemHelper.h"

void URTSProgressionAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME(UMyAttributeSet, MyAttribute);
    // Chances are this is how you would ordinarily do it, however in the case of attributes this'll lead
    // to confusing and annoying replication errors, usually involving clientside ability prediction.
    // https://wiki.unrealengine.com/GameplayAbilities_and_You#AttributeSet
    // This is how it is done properly for attributes.
    DOREPLIFETIME_CONDITION_NOTIFY(URTSProgressionAttributeSet, Level, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(URTSProgressionAttributeSet, CurrentExp, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSProgressionAttributeSet, ExpReward, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSProgressionAttributeSet, SkillPoints, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSProgressionAttributeSet, StatPoints, COND_None, REPNOTIFY_Always);
}

URTSProgressionAttributeSet::URTSProgressionAttributeSet()
{
    Level = 0.0f;
    CurrentExp = 0.0f;
	ExpReward = 10.0f;
	SkillPoints = 1.0f;
	StatPoints = 0.0f;
}

void URTSProgressionAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    // Level up every 100 exp
    if (Data.EvaluatedData.Attribute == GetCurrentExpAttribute())
    {
        while (GetCurrentExp() >= 100)
        {
            SetCurrentExp(GetCurrentExp() - 100);
            SetLevel(GetLevel() + 1);
            SetSkillPoints(GetSkillPoints() + 1);
            FGameplayEventData LevelUpEventData;
            URTSAbilitySystemHelper::GetSourceAndTargetTags(GetOwningActor(), GetOwningActor(), LevelUpEventData.InstigatorTags, LevelUpEventData.TargetTags);
            UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), URTSGlobalTags::Event_OnLevelUp(), LevelUpEventData);
        }
    }
}
