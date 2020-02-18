// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameplayEffect.h"
#include "AttributeSet.h"
#include "GenericTeamAgentInterface.h"
#include "OrdersAbilitiesCharacter.generated.h"

class URTSAbilitySystemComponent;
class URTSOrderComponent;
class URTSAutoOrderComponent;

UCLASS()
class ORDERSABILITIES_API AOrdersAbilitiesCharacter
	: public ACharacter
	, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/** Our ability system */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	URTSAbilitySystemComponent* AbilitySystem;

	/** Order component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Orders, meta = (AllowPrivateAccess = "true"))
	URTSOrderComponent* OrderComponent;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Orders, meta = (AllowPrivateAccess = "true"))
	// URTSAutoOrderComponent* AutoOrderComponent;
	
	// Sets default values for this character's properties
	AOrdersAbilitiesCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UNetConnection* GetNetConnection() const override;
};
