#include "AbilitySystem/RTSDefenseAttributeSet.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystem/RTSGlobalTags.h"
#include "AbilitySystemBlueprintLibrary.h"

#include "Math/UnrealMathUtility.h"

#include "GameplayTagsManager.h"
#include "GameplayEffectExtension.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/RTSAbilitySystemHelper.h"

const float URTSDefenseAttributeSet::MAX_HEALTH = 10000.0f;
const float URTSDefenseAttributeSet::MAX_PHYSICAL_ARMOR = 10000.0f;
const float URTSDefenseAttributeSet::MAX_MAGICAL_ARMOR = 10000.0f;
const float URTSDefenseAttributeSet::MAX_INCOMING_DAMAGE_MULTIPLIER = 10000.0f;


void URTSDefenseAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // DOREPLIFETIME(UMyAttributeSet, MyAttribute);
    // Chances are this is how you would ordinarily do it, however in the case of attributes this'll lead
    // to confusing and annoying replication errors, usually involving clientside ability prediction.
    // https://wiki.unrealengine.com/GameplayAbilities_and_You#AttributeSet
    // This is how it is done properly for attributes.
    DOREPLIFETIME_CONDITION_NOTIFY(URTSDefenseAttributeSet, Damage, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSDefenseAttributeSet, Health, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSDefenseAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSDefenseAttributeSet, PhysicalArmor, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSDefenseAttributeSet, MagicalArmor, COND_None, REPNOTIFY_Always);
    DOREPLIFETIME_CONDITION_NOTIFY(URTSDefenseAttributeSet, IncomingDamageMultiplier, COND_None, REPNOTIFY_Always);
}

URTSDefenseAttributeSet::URTSDefenseAttributeSet()
{
	InitDamage(0.0f);
	InitHealth(100.0f);
	InitMaxHealth(100.0f);
	InitPhysicalArmor(0.0f);
	InitMagicalArmor(0.0f);
	InitIncomingDamageMultiplier(1.0f);

	// GetOwningAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetMaxHealthAttribute()).AddUObject(this, &URTSDefenseAttributeSet::OnMaxHealthChanged);
}

void URTSDefenseAttributeSet::Init()
{
	GetOwningAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetMaxHealthAttribute()).AddUObject(this, &URTSDefenseAttributeSet::OnMaxHealthChanged);
}

bool URTSDefenseAttributeSet::ShouldInitProperty(bool FirstInit, FProperty* PropertyToInit) const
{
    // We do not want the health property to change when the attribute sets properties where initialized using a curve
    // table.

	return true;// (PropertyToInit != GetIncomingDamageMultiplierAttribute().GetUProperty());
}

void URTSDefenseAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}

	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, MAX_HEALTH);
	}

	if (Attribute == GetPhysicalArmorAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, MAX_PHYSICAL_ARMOR);
	}

	if (Attribute == GetMagicalArmorAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, MAX_MAGICAL_ARMOR);
	}

	if (Attribute == GetIncomingDamageMultiplierAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, MAX_INCOMING_DAMAGE_MULTIPLIER);
	}
}

void URTSDefenseAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	UAttributeSet::PreAttributeChange(Attribute, NewValue);
}

void URTSDefenseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	//----------------
	//	This doesn't get called by effects which have infinite duration
	//----------------

	FGameplayEffectContextHandle Context = Data.EffectSpec.GetContext();
	UAbilitySystemComponent* Source = Context.GetOriginalInstigatorAbilitySystemComponent();

	const FGameplayTagContainer& SourceTags = *Data.EffectSpec.CapturedSourceTags.GetAggregatedTags();

	// Get the Source actor
	AActor* SourceActor = nullptr;
	if (Source && Source->AbilityActorInfo.IsValid() && Source->AbilityActorInfo->AvatarActor.IsValid())
	{
		// Set the causer actor based on context if it's set
		if (Context.GetEffectCauser())
		{
			SourceActor = Context.GetEffectCauser();
		}
	}

	// if (Data.EvaluatedData.Attribute == GetMaxHealthAttribute())
	// {
	// 	const float OldMaxHealth = GetMaxHealth() - Data.EvaluatedData.Magnitude;
	// 	float OldPercentHealth = OldMaxHealth / GetHealth();
	// 	float NewHealth = OldPercentHealth * GetMaxHealth();
	// 	SetHealth(NewHealth);
	// }
	
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
		{
			// you can only lose as much health as you have
			const float OldHealth = GetHealth() - Data.EvaluatedData.Magnitude;

			float HealthChange = Data.EvaluatedData.Magnitude;

			if (HealthChange < 0) // lost health
			{
				HandleHealthLost(SourceActor, -1 * HealthChange, OldHealth);
			}
			else if (HealthChange > 0) // gained health
			{
				if (GetHealth() > GetMaxHealth()) // over heal!
				{
					const float Overheal = GetHealth() - GetMaxHealth();
					HealthChange -= Overheal;
					SetHealth(GetMaxHealth());
				}

				// FGameplayEventData DataHealthGained;
				// DataHealthGained.EventMagnitude = HealthChange;
				// UAbilitySystemBlueprintLibrary::SendGameplayEventWithTargetActor(GetOwningActor(), FGameplayTag::RequestGameplayTag(FName("Event.Health.Gained"), false), DataHealthGained);
			}
		}
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// Store a local copy of the amount of damage done and clear the damage attribute
		float LocalDamageDone = GetDamage();
		SetDamage(0.f);

		FGameplayTagContainer EffectTags;
		Data.EffectSpec.GetAllAssetTags(EffectTags);

		if (EffectTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Damage.Physical"), false)))
		{
			const float PhysicalDamageReduction = (0.052f * GetPhysicalArmor()) / (0.9 + 0.048 * GetPhysicalArmor());
			LocalDamageDone = LocalDamageDone * (1 - PhysicalDamageReduction);// -GetPhysicalBlock();
		}

		if (EffectTags.HasTag(FGameplayTag::RequestGameplayTag(TEXT("Damage.Magical"), false)))
		{
			const float MagicalDamageReduction = (0.052f * GetMagicalArmor()) / (0.9 + 0.048 * GetMagicalArmor());
			LocalDamageDone = LocalDamageDone * (1 - MagicalDamageReduction);// -GetMagicalBlock();
		}

		if (LocalDamageDone > 0)
		{
			HandleHealthLost(SourceActor, LocalDamageDone, GetHealth());
		}
	}
}

void URTSDefenseAttributeSet::PostInitializeProperties(bool bInitialInit)
{
  //   if (bInitialInit)
  //   {
  //       IncomingDamageMultiplier = 1.0f;
		// SetHealth(GetMaxHealth());
		// UE_LOG(LogTemp, Warning, TEXT("%s Set health to %f"), TEXT(__FUNCTION__), GetHealth());
  //   }
  //
	// if (bInitialInit)
	// {
	// 	GetOwningAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetMaxHealthAttribute()).AddUObject(this, &URTSDefenseAttributeSet::OnMaxHealthChanged);
	// }
}

void URTSDefenseAttributeSet::GetDefaultStatusTags(FGameplayTagContainer& OutStatusTags) const
{
    OutStatusTags.AddTag(URTSGlobalTags::Status_Permanent_CanBeAttacked());
    OutStatusTags.AddTag(URTSGlobalTags::Status_Changing_IsAlive());
}

void URTSDefenseAttributeSet::AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const
{
	UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilityComp)
	{
		// Change current value to maintain the current Val / Max percent
		const float CurrentValue = AffectedAttribute.GetCurrentValue();
		float NewDelta = (CurrentMaxValue > 0.f) ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;

		AbilityComp->ApplyModToAttribute(AffectedAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void URTSDefenseAttributeSet::HandleHealthLost(AActor* SourceActor, float HealthLost, const float HealthBeforeLost)
{
	FGameplayEventData DataHealthLost;
	DataHealthLost.OptionalObject = SourceActor;
	DataHealthLost.EventMagnitude = HealthLost;
	URTSAbilitySystemHelper::GetSourceAndTargetTags(GetOwningActor(), GetOwningActor(), DataHealthLost.InstigatorTags, DataHealthLost.TargetTags);
	
	if (HealthLost >= HealthBeforeLost) // we died
	{
		if (HealthLost > HealthBeforeLost)
		{
			float Overkill = HealthLost - HealthBeforeLost;
			HealthLost -= Overkill;
		}

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), URTSGlobalTags::Event_OnKilled(), DataHealthLost);
		
	}

	SetHealth(HealthBeforeLost - HealthLost);

	if (HealthLost > 0)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningActor(), FGameplayTag::RequestGameplayTag(FName("Event.Health.Lost"), false), DataHealthLost);
	
		if (SourceActor != nullptr)
		{
			FGameplayEventData DataDamageDealt;
			DataDamageDealt.OptionalObject = GetOwningActor();
			DataDamageDealt.EventMagnitude = HealthLost;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceActor, FGameplayTag::RequestGameplayTag(FName("Event.Damage.Dealt"), false), DataDamageDealt);
		}
	}
	
}

void URTSDefenseAttributeSet::OnMaxHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	const float OldPercentHealth = GetHealth() / ChangeData.OldValue;
	const float NewHealth = OldPercentHealth * ChangeData.NewValue;
	SetHealth(NewHealth);
	// UE_LOG(LogTemp, Warning, TEXT("%s Set health to %f (%f) : %f"), TEXT(__FUNCTION__), NewHealth, GetHealth(), ChangeData.NewValue);
}
