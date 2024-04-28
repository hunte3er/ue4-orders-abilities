// Fill out your copyright notice in the Description page of Project Settings.

#include "OrdersAbilitiesCharacter.h"
#include "AbilitySystem/RTSAbilitySystemComponent.h"
#include "Orders/RTSOrderComponent.h"
#include "Orders/RTSAutoOrderComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
AOrdersAbilitiesCharacter::AOrdersAbilitiesCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystem = CreateDefaultSubobject<URTSAbilitySystemComponent>(TEXT("RTSAbilitySystem"));

	// Minimal Mode means that no GameplayEffects will replicate. They will only live on the Server. Attributes, GameplayTags, and GameplayCues will still replicate to us.
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	OrderComponent = CreateDefaultSubobject<URTSOrderComponent>(TEXT("RTSOrderComponent"));
	//AutoOrderComponent = CreateDefaultSubobject<URTSAutoOrderComponent>(TEXT("RTSAutoOrderComponent"));
}

// Called when the game starts or when spawned
void AOrdersAbilitiesCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Initialize actor info
	AbilitySystem->InitAbilityActorInfo(this, this);
}

// Called every frame
void AOrdersAbilitiesCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AOrdersAbilitiesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAbilitySystemComponent* AOrdersAbilitiesCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

UNetConnection* AOrdersAbilitiesCharacter::GetNetConnection() const
{
	return Super::GetNetConnection();
}

