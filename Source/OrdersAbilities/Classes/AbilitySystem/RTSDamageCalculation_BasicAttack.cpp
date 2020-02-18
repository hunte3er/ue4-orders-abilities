// Fill out your copyright notice in the Description page of Project Settings.

#include "AbilitySystem/RTSDamageCalculation_BasicAttack.h"
#include "AbilitySystem/RTSAttackAttributeSet.h"
#include "AbilitySystem/RTSDefenseAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

struct RTSDamageStatics_BasicAttack
{
	/*DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);*/

	// source's offensive stats
	// DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(DamageAdditive);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(DamageMultiplier);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeChance);
	// DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalStrikeMultiplier);
	//
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackDamageLow);
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackDamageHigh);

	// target's defensive stats
	// DECLARE_ATTRIBUTE_CAPTUREDEF(IncomingDamageMultiplier);

	RTSDamageStatics_BasicAttack()
	{
		// Capture the Target's DefensePower attribute. Do not snapshot it, because we want to use the health value at the moment we apply the execution.
		//DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, DefensePower, Target, false);

		// Capture the Source's AttackPower. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		// the projectile was launched, not when it hits).
		//DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttributeSet, AttackPower, Source, true);

		// Capture the Source's DamageAdditive. We do want to snapshot this at the moment we create the GameplayEffectSpec that will execute the damage.
		// (imagine we fire a projectile: we create the GE Spec when the projectile is fired. When it hits the target, we want to use the AttackPower at the moment
		// the projectile was launched, not when it hits).

		// /* 
		// * Capture the Source's Offensive Stats
		// */ 
		// DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttackAttributeSet, DamageAdditive, Source, true);
		// DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttackAttributeSet, DamageMultiplier, Source, true);
		//
		// DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttackAttributeSet, CriticalStrikeChance, Source, true);
		// DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttackAttributeSet, CriticalStrikeMultiplier, Source, true);

		// Also capture the source's raw Damage, which is normally passed in directly via the execution
		// DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttackAttributeSet, Damage, Source, true);

		DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttackAttributeSet, AttackDamageLow, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URTSAttackAttributeSet, AttackDamageHigh, Source, true);

		/*
		* Capture the Target's Defensive Stats
		*/
		// DEFINE_ATTRIBUTE_CAPTUREDEF(URTSDefenseAttributeSet, IncomingDamageMultiplier, Target, true);
	}
};

static const RTSDamageStatics_BasicAttack& DamageStatics()
{
	static RTSDamageStatics_BasicAttack DmgStatics;
	return DmgStatics;
}

URTSDamageCalculation_BasicAttack::URTSDamageCalculation_BasicAttack()
{
	// source's stats
	// RelevantAttributesToCapture.Add(DamageStatics().DamageAdditiveDef);
	// RelevantAttributesToCapture.Add(DamageStatics().DamageMultiplierDef);
	// RelevantAttributesToCapture.Add(DamageStatics().CriticalStrikeChanceDef);
	// RelevantAttributesToCapture.Add(DamageStatics().CriticalStrikeMultiplierDef);
	// RelevantAttributesToCapture.Add(DamageStatics().DamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().AttackDamageLowDef);
	RelevantAttributesToCapture.Add(DamageStatics().AttackDamageHighDef);

	// target's stats
	// RelevantAttributesToCapture.Add(DamageStatics().IncomingDamageMultiplierDef);
}

void URTSDamageCalculation_BasicAttack::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, OUT FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* SourceActor = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->AvatarActor : nullptr;
	AActor* TargetActor = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->AvatarActor : nullptr;

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	// --------------------------------------
	//	Damage Done = Damage * AttackPower / DefensePower
	//	If DefensePower is 0, it is treated as 1.0
	// --------------------------------------

	// --------------------------------------
	//	Damage Done = (Damage + DamageAdditive) * DamageMultiplier
	// --------------------------------------

	// float DamageMultiplier = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageMultiplierDef, EvaluationParameters, DamageMultiplier);
	//
	// float DamageAdditive = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageAdditiveDef, EvaluationParameters, DamageAdditive);

	// float Damage = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);

	float AttackDamageLow = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDamageLowDef, EvaluationParameters, AttackDamageLow);
	
	float AttackDamageHigh = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDamageHighDef, EvaluationParameters, AttackDamageHigh);
	//
	// float CriticalStrikeChance = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalStrikeChanceDef, EvaluationParameters, CriticalStrikeChance);
	//
	// float CriticalStrikeMultiplier = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalStrikeMultiplierDef, EvaluationParameters, CriticalStrikeMultiplier);

	// float IncomingDamageMultiplier = 0.f;
	// ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().IncomingDamageMultiplierDef, EvaluationParameters, IncomingDamageMultiplier);

	const float AttackDamage = FMath::RandRange(AttackDamageLow, AttackDamageHigh);
	// float DamageDone = (AttackDamage + Damage + DamageAdditive) * DamageMultiplier * IncomingDamageMultiplier;
	// if (DamageDone > 0.f)
	// {
	// 	if (FMath::RandRange(0.f, 1.f) <= CriticalStrikeChance)
	// 	{
	// 		DamageDone *= CriticalStrikeMultiplier;
	// 	}
	//
	// 	// apply backstab passive
	// 	if (SourceAbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Passive.Backstab"), false)))
	// 	{
	// 		if (SourceActor && TargetActor)
	// 		{
	// 			if ((SourceActor->GetActorLocation() - TargetActor->GetActorLocation()).Size2D() < 200)
	// 			{
	// 				const float Angle = FMath::Acos(FVector::DotProduct(SourceActor->GetActorForwardVector(), TargetActor->GetActorForwardVector()));
	// 				const float AngleInDegree = FMath::RadiansToDegrees(Angle);
	// 				if (AngleInDegree < 30)
	// 				{
	// 					DamageDone *= 2.f;
	// 				}
	// 			}
	// 		}
	// 	}
	//
	// 	if (TargetAbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("Status.Buff.Strong.Counter"), false)))
	// 	{
	// 		OutExecutionOutput.MarkGameplayCuesHandledManually();
	// 	}
	//
	// 	// Handle countering
	// 	// TODO: account for DoT
	// 	// take no damage if you are countering
	// 	const bool bIsCountering = TargetAbilitySystemComponent->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Status.Buff.Strong.Counter"), false));
	// 	if (bIsCountering)
	// 	{
	// 		FGameplayEventData DataCounter;
	// 		DataCounter.OptionalObject = SourceActor;
	// 		DataCounter.EventMagnitude = DamageDone;
	// 		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, FGameplayTag::RequestGameplayTag(FName("Event.Counter"), false), DataCounter);
	// 	}
	// 	else
	// 	{
	// 		// successfully hit the target
	// 		FGameplayEventData DataBasicAttack;
	// 		DataBasicAttack.OptionalObject = TargetActor;
	// 		DataBasicAttack.EventMagnitude = DamageDone;
	// 		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(SourceActor, FGameplayTag::RequestGameplayTag(FName("Event.BasicAttack"), false), DataBasicAttack);
	//
	// 		OutExecutionOutput.MarkConditionalGameplayEffectsToTrigger();
	// 		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(DamageStatics().DamageProperty, EGameplayModOp::Additive, DamageDone));
	// 	}
	// }

	OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(URTSDefenseAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, AttackDamage));
}