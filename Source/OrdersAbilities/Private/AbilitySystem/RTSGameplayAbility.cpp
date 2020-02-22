#include "AbilitySystem/RTSGameplayAbility.h"
#include "AbilitySystem/RTSGlobalTags.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/RTSAbilitySystemHelper.h"
#include "AbilitySystem/RTSAttackAttributeSet.h"
#include "AbilitySystem/RTSResourceAttributeSet.h"

URTSGameplayAbility::URTSGameplayAbility(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // TODO: Is their any reason a RTS game play ability should always be server only?
    NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;

    // This setting should always be false for an RTS. All abilities are fired or canceled on the server.
    bServerRespectsRemoteAbilityCancellation = false;

    ReplicationPolicy = EGameplayAbilityReplicationPolicy::ReplicateNo;

    AbilityProcessPolicy = ERTSAbilityProcessPolicy::INSTANT;

    GroupExecutionType = ERTSOrderGroupExecutionType::MOST_SUITABLE_UNIT;

    AcquisitionRadiusOverride = 0.0f;
    bIsAcquisitionRadiusOverridden = false;

    bHumanPlayerAutoAbility = false;
    bHumanPlayerAutoAutoAbilityInitialState = false;
    bAIPlayerAutoAbility = true;

	SourceRequiredTags.AddTag(URTSGlobalTags::Status_Changing_IsAlive()); 
	TargetRequiredTags.AddTag(URTSGlobalTags::Status_Changing_IsAlive());
}

bool URTSGameplayAbility::IsTargetTypeFlagChecked(int32 InFlag) const
{
	// UE_LOG(LogTemp, Warning, TEXT("%d | %d"), TargetTypeFlags, InFlag);
    return (TargetTypeFlags & InFlag) != 0;
}

bool URTSGameplayAbility::IsTargetTypeFlagChecked(ERTSTargetTypeFlags InFlag) const
{
    return IsTargetTypeFlagChecked(StaticCast<int32>(InFlag));
}

ERTSOrderGroupExecutionType URTSGameplayAbility::GetGroupExecutionType() const
{
    return GroupExecutionType;
}

FGameplayTag URTSGameplayAbility::GetEventTriggerTag() const
{
    for (const FAbilityTriggerData& AbilityTrigger : AbilityTriggers)
    {
        if (AbilityTrigger.TriggerSource == EGameplayAbilityTriggerSource::GameplayEvent)
        {
            return AbilityTrigger.TriggerTag;
        }
    }

    return FGameplayTag();
}

const TArray<FAbilityTriggerData>& URTSGameplayAbility::GetAbilityTriggerData() const
{
    return AbilityTriggers;
}

UTexture2D* URTSGameplayAbility::GetIcon() const
{
    return Icon;
}

FText URTSGameplayAbility::GetName() const
{
    return Name;
}

FText URTSGameplayAbility::GetDescription(const AActor* Actor) const
{
    FText FormattedDescription;
    FormatDescription(Description, Actor, FormattedDescription);
    return FormattedDescription;
}

int32 URTSGameplayAbility::GetMaxLevel() const
{
    return MaxLevel;
}

FRTSOrderPreviewData URTSGameplayAbility::GetAbilityPreviewData() const
{
    return AbilityPreviewData;
}

void URTSGameplayAbility::FormatDescription_Implementation(const FText& InDescription, const AActor* Actor,
                                                           FText& OutDescription) const

{
    OutDescription = InDescription;
}

bool URTSGameplayAbility::ShouldShowAsOrderInUI()
{
    return bShouldShowAsOrderInUI;
}

bool URTSGameplayAbility::GetAcquisitionRadiusOverride(float& OutAcquisitionRadius) const
{
    OutAcquisitionRadius = AcquisitionRadiusOverride;
    return bIsAcquisitionRadiusOverridden;
}

bool URTSGameplayAbility::IsHumanPlayerAutoAbility() const
{
    return bHumanPlayerAutoAbility;
}

bool URTSGameplayAbility::GetHumanPlayerAutoAutoAbilityInitialState() const
{
    return bHumanPlayerAutoAutoAbilityInitialState;
}

bool URTSGameplayAbility::IsAIPlayerAutoAbility() const
{
    return bAIPlayerAutoAbility;
}

bool URTSGameplayAbility::IsTargetScoreOverriden() const
{
    return bIsTargetScoreOverridden;
}

bool URTSGameplayAbility::AreAbilityTasksActive() const
{
    return ActiveTasks.Num() > 0;
}

void URTSGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
    if (TriggerEventData)
    {
        CurrentEventData = *TriggerEventData;
    }
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

bool URTSGameplayAbility::ShouldActivateAbility(ENetRole Role) const
{
    // This is currently only used by CanActivateAbility to block clients from activating abilities themselves.
    // This in turn is also prevented by NetExecutionPolicy in our case.
    // However, CanActivateAbility is also used by clients in order to check the respective ability before issuing the
    // order.
    return true;
}

void URTSGameplayAbility::OnGameplayTaskActivated(UGameplayTask& Task)
{
    Super::OnGameplayTaskActivated(Task);
}

void URTSGameplayAbility::OnGameplayTaskDeactivated(UGameplayTask& Task)
{
    Super::OnGameplayTaskDeactivated(Task);
}

void URTSGameplayAbility::OnAbilityLevelChanged_Implementation(int32 NewLevel)
{
}

void URTSGameplayAbility::GetTargetScore_Implementation(const AActor* OrderedActor, const FRTSOrderTargetData& TargetData, int32 Index, float& OutScore) const
{
    OutScore = 1.0f;
}

float URTSGameplayAbility::GetRange(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo) const
{
    return GetRange();
}

ERTSAbilityProcessPolicy URTSGameplayAbility::GetAbilityProcessPolicy() const
{
    return AbilityProcessPolicy;
}

FGameplayTagContainer URTSGameplayAbility::GetActivationRequiredTags() const
{
    return ActivationRequiredTags;
}

void URTSGameplayAbility::GetOrderTagRequirements(FRTSOrderTagRequirements& OutTagRequirements) const
{
    OutTagRequirements.SourceRequiredTags.AppendTags(SourceRequiredTags);
    OutTagRequirements.SourceBlockedTags.AppendTags(SourceBlockedTags);
    OutTagRequirements.TargetRequiredTags.AppendTags(TargetRequiredTags);
    OutTagRequirements.TargetBlockedTags.AppendTags(TargetBlockedTags);
}

void URTSGameplayAbility::GetSourceTagRequirements(FGameplayTagContainer& OutRequiredTags,
                                                   FGameplayTagContainer& OutBlockedTags) const
{
    OutRequiredTags.AppendTags(SourceRequiredTags);
    OutBlockedTags.AppendTags(SourceBlockedTags);
}

void URTSGameplayAbility::GetTargetTagRequirements(FGameplayTagContainer& OutRequiredTags,
                                                   FGameplayTagContainer& OutBlockedTags) const
{
    OutRequiredTags.AppendTags(TargetRequiredTags);
    OutBlockedTags.AppendTags(TargetBlockedTags);
}


//////////////
// Cooldown //
//////////////
void URTSGameplayAbility::FPGAApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		// CooldownGE->InheritableOwnedTagsContainer.CombinedTags = CooldownTags;// .AddTag(AbilityTag);

		auto AbilitySystem = ActorInfo->AbilitySystemComponent;
		
		FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(CooldownGameplayEffectClass, GetAbilityLevel(Handle, ActorInfo), MakeEffectContext(Handle, ActorInfo));
		// FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(CooldownGameplayEffectClass, GetAbilityLevel(Handle, ActorInfo), FGameplayEffectContextHandle()); //MakeEffectContext(Handle, ActorInfo));
		// FGameplayAbilitySpecHandle* NewDummyHandle = new FGameplayAbilitySpecHandle();
		// FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(*NewDummyHandle, ActorInfo, ActivationInfo, CooldownGameplayEffectClass, GetAbilityLevel(Handle, ActorInfo));
		if (SpecHandle.IsValid() && SpecHandle.Data.IsValid())
		{
			SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
			
			const float CooldownMagnitude = GetAbilityCooldown(ActorInfo->AbilitySystemComponent.Get(), Handle);

			SpecHandle.Data.Get()->SetSetByCallerMagnitude(URTSGlobalTags::SetByCaller_Cooldown(), CooldownMagnitude);

			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
		}
	}
}

bool URTSGameplayAbility::HasCooldown()
{
	return CooldownTags.Num() > 0;
}

float URTSGameplayAbility::GetAbilityBaseCooldown(int32 Level) const
{
	static const FString CalculateMagnitudeContext(TEXT("FAttributeBasedFloat::CalculateMagnitude"));

	float CooldownMagnitude = AbilityCooldown;
	
	if (AbilityCooldownCurve.IsValid(CalculateMagnitudeContext))
	{
		AbilityCooldownCurve.Eval(Level, &CooldownMagnitude, CalculateMagnitudeContext);
	}

	return CooldownMagnitude;
}

void URTSGameplayAbility::CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	FPGAApplyCooldown(Handle, ActorInfo, ActivationInfo);

	ApplyCost(Handle, ActorInfo, ActivationInfo);
}

bool URTSGameplayAbility::CommitAbilityCooldown(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const bool ForceCooldown)
{
	if (UAbilitySystemGlobals::Get().ShouldIgnoreCooldowns())
	{
		return true;
	}

	if (!ForceCooldown)
	{
		// Last chance to fail (maybe we no longer have resources to commit since we after we started this ability activation)
		if (!CheckCooldown(Handle, ActorInfo))
		{
			return false;
		}
	}

	FPGAApplyCooldown(Handle, ActorInfo, ActivationInfo);
	return true;
}


bool URTSGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (CostGE)
	{
		UAbilitySystemComponent* const AbilitySystemComponent = ActorInfo->AbilitySystemComponent.Get();
		check(AbilitySystemComponent != nullptr);

		FGameplayEffectSpec Spec(CostGE, MakeEffectContext(Handle, ActorInfo), GetAbilityLevel(Handle, ActorInfo));
		ApplyCostMagnitude(ActorInfo->AbilitySystemComponent.Get(), Spec, Handle);
		Spec.CalculateModifierMagnitudes();

		if (!CanApplySpecAttributeModifiers(AbilitySystemComponent, Spec, Handle))
		{
			const FGameplayTag& FailTag = UAbilitySystemGlobals::Get().ActivateFailCostTag;

			if (OptionalRelevantTags && FailTag.IsValid())
			{
				OptionalRelevantTags->AddTag(FailTag);
			}
			return false;
		}
	}
	
	return true;
}

void URTSGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CostGE = GetCostGameplayEffect();
	if (CostGE)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CostGameplayEffectClass, GetAbilityLevel(Handle, ActorInfo));
		if (SpecHandle.IsValid() && SpecHandle.Data.IsValid())
		{
			FGameplayEffectSpec& Spec = *SpecHandle.Data.Get();

			// float CostMagnitude = AbilityCost;
			//
			// if (UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo())
			// {
			// 	CostMagnitude /= 1.0f + AbilitySystem->GetNumericAttribute(URTSResourceAttributeSet::GetCostReductionAttribute());
			// }
			//
			// Spec.SetSetByCallerMagnitude(URTSGlobalTags::SetByCaller_Cost_Mana(), CostMagnitude);

			ApplyCostMagnitude(ActorInfo->AbilitySystemComponent.Get(), Spec, Handle);
			ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Error: Spec handle or data is invalid"));
		}
	}
}

float URTSGameplayAbility::GetAbilityCooldown_Implementation(UAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpecHandle& Handle) const
{
	FGameplayAbilitySpec* Spec = AbilitySystem->FindAbilitySpecFromHandle(Handle);

	int32 AbilityLevel = Spec ? Spec->Level : 0;
	
	float CooldownMagnitude = GetAbilityBaseCooldown(AbilityLevel);
	
	if (AbilitySystem)
	{
		CooldownMagnitude /= 1.0f + AbilitySystem->GetNumericAttribute(URTSAttackAttributeSet::GetCooldownReductionAttribute());
	}

	return CooldownMagnitude;
}

float URTSGameplayAbility::GetAbilityCost_Implementation(UAbilitySystemComponent* AbilitySystem, const FGameplayAbilitySpecHandle& Handle) const
{
	float CostMagnitude = AbilityCost;

	// if a curve table entry is specified, use the attribute value as a lookup into the curve instead of using it directly
	static const FString CalculateMagnitudeContext(TEXT("FAttributeBasedFloat::CalculateMagnitude"));
	
	if (FGameplayAbilitySpec* Spec = AbilitySystem->FindAbilitySpecFromHandle(Handle))
	{
		if (AbilityCostCurve.IsValid(CalculateMagnitudeContext))
		{
			AbilityCostCurve.Eval(Spec->Level, &CostMagnitude, CalculateMagnitudeContext);
		}
	}

	if (AbilitySystem)
	{
		CostMagnitude /= 1.0f + AbilitySystem->GetNumericAttribute(URTSResourceAttributeSet::GetCostReductionAttribute());
	}

	return CostMagnitude;
}

const FGameplayTagContainer* URTSGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

bool URTSGameplayAbility::CanApplySpecAttributeModifiers(UAbilitySystemComponent* AbilitySystem, const FGameplayEffectSpec& Spec, const FGameplayAbilitySpecHandle& Handle) const
{
	for (int32 ModIdx = 0; ModIdx < Spec.Modifiers.Num(); ++ModIdx)
	{
		const FGameplayModifierInfo& ModDef = Spec.Def->Modifiers[ModIdx];
		const FModifierSpec& ModSpec = Spec.Modifiers[ModIdx];

		// It only makes sense to check additive operators
		if (ModDef.ModifierOp == EGameplayModOp::Additive)
		{
			if (!ModDef.Attribute.IsValid())
			{
				continue;
			}

			const float CostValue = ModSpec.GetEvaluatedMagnitude();
			if (CostValue == 0)
			{
				continue;
			}
			
			UAttributeSet* Set = URTSAbilitySystemHelper::FindAttributeSetOfClass(AbilitySystem, ModDef.Attribute.GetAttributeSetClass());
			if (Set == nullptr)
			{
				return false;
			}

			const float CurrentValue = ModDef.Attribute.GetNumericValueChecked(Set);

			if (CurrentValue + CostValue < 0.f)
			{
				return false;
			}
		}
	}

	return true;
}

void URTSGameplayAbility::ApplyCostMagnitude(UAbilitySystemComponent* AbilitySystem, FGameplayEffectSpec& Spec, const FGameplayAbilitySpecHandle& Handle) const
{
	check(AbilitySystem);

	const float CostMagnitude = GetAbilityCost(AbilitySystem, Handle);

	//UE_LOG(LogTemp, Warning, TEXT("Cost: %f"), CostMagnitude);

	Spec.SetSetByCallerMagnitude(URTSGlobalTags::SetByCaller_Cost_Mana(), -1.0f * CostMagnitude);

	Spec.CalculateModifierMagnitudes();
}