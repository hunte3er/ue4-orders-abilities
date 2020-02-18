#include "RTSOwnerComponent.h"
#include "RTSPlayerState.h"
#include "RTSTeamInfo.h"
#include "RTSOwnerComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Controller.h"

URTSOwnerComponent::URTSOwnerComponent(const FObjectInitializer& ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
}

void URTSOwnerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URTSOwnerComponent, PlayerOwner);
}


ARTSPlayerState* URTSOwnerComponent::GetPlayerOwner() const
{
	return PlayerOwner;
}

void URTSOwnerComponent::SetPlayerOwner(AController* Controller)
{
	ARTSPlayerState* PreviousOwner = PlayerOwner;

	if (!Controller)
	{
		PlayerOwner = nullptr;
	}
	else
	{
		PlayerOwner = Cast<ARTSPlayerState>(Controller->PlayerState);
	}

	if (PlayerOwner != PreviousOwner)
	{
		AActor* Actor = GetOwner();
		if (PreviousOwner != nullptr)
		{
			PreviousOwner->RemoveOwnedActor(Actor);
		}
		PlayerOwner->AddOwnedActor(Actor);
		
		OnOwnerChanged.Broadcast(PreviousOwner, PlayerOwner);
	}
}

bool URTSOwnerComponent::IsSameTeamAsActor(AActor* Other)
{
	if (!Other)
	{
		return false;
	}

	ARTSPlayerState* MyOwner = GetPlayerOwner();

	URTSOwnerComponent* OtherOwnerComponent = Other->FindComponentByClass<URTSOwnerComponent>();

	if (!OtherOwnerComponent)
	{
		return false;
	}

	ARTSPlayerState* OtherOwner = OtherOwnerComponent->GetPlayerOwner();

	if (!MyOwner && !OtherOwner)
	{
		return true;
	}
	
	if (!MyOwner || !OtherOwner)
	{	
		return false;
	}

	return MyOwner->IsSameTeamAs(OtherOwner);
}

bool URTSOwnerComponent::IsSameTeamAsController(AController* C)
{
	ARTSPlayerState* MyOwner = GetPlayerOwner();
	ARTSPlayerState* OtherPlayer = Cast<ARTSPlayerState>(C->PlayerState);

	if (MyOwner == nullptr && OtherPlayer == nullptr)
	{
		return true;
	}

	return MyOwner && MyOwner->IsSameTeamAs(OtherPlayer);
}
