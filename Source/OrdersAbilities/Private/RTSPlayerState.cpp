
#include "RTSPlayerState.h"
#include "OrdersAbilities/OrdersAbilities.h"
#include "RTSPlayerController.h"
#include "RTSTeamInfo.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystem/RTSAbilitySystemComponent.h"

ARTSPlayerState::ARTSPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetUpdateFrequency = 100.0f;
	
	AbilitySystem = CreateDefaultSubobject<URTSAbilitySystemComponent>(TEXT("RTSAbilitySystem"));
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void ARTSPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// Initialize actor info
	AbilitySystem->InitAbilityActorInfo(this, this);
}

bool ARTSPlayerState::IsSameTeamAs(const ARTSPlayerState* Other) const
{
	if (!Other)
	{
		return false;
	}

	ARTSTeamInfo* FirstTeam = Team;
	ARTSTeamInfo* SecondTeam = Other->Team;

	if (!FirstTeam || !SecondTeam)
	{
		return false;
	}

	return FirstTeam->TeamIndex == SecondTeam->TeamIndex;
}

void ARTSPlayerState::OnTeamChanged()
{
	NotifyOnTeamChanged(Team);
}

void ARTSPlayerState::NotifyOnTeamChanged(ARTSTeamInfo* NewTeam)
{
	if (NewTeam)
	{
		UE_LOG(LogRTS, Log, TEXT("Player %s added to team %d."), *GetName(), NewTeam->TeamIndex);
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("Player %s added to team None."), *GetName());
	}

	// Notify listeners.
	ReceiveOnTeamChanged(NewTeam);

	// Notify player.
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(GetOwner());

	if (PlayerController)
	{
		PlayerController->NotifyOnTeamChanged(NewTeam);
	}
}

void ARTSPlayerState::AddOwnedActor(AActor* Actor)
{
	OwnedActors.Add(Actor);
	OnPartyActorChanged.Broadcast(Actor, ERTSPartyActorChangeEvent::PARTYACTOR_ADDED);
}

void ARTSPlayerState::RemoveOwnedActor(AActor* Actor)
{
	OwnedActors.Remove(Actor);
	OnPartyActorChanged.Broadcast(Actor, ERTSPartyActorChangeEvent::PARTYACTOR_REMOVED);
}

UAbilitySystemComponent* ARTSPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void ARTSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTSPlayerState, Team);
	DOREPLIFETIME(ARTSPlayerState, OwnedActors);
}
