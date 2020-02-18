#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"

#include "RTSPlayerState.generated.h"

UENUM(BlueprintType)
enum class ERTSPartyActorChangeEvent : uint8
{
	PARTYACTOR_ADDED,
	PARTYACTOR_REMOVED,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSOwnedActorsChanged, AActor*, Actor, ERTSPartyActorChangeEvent, ChangeEvent);

class ARTSTeamInfo;

class URTSAbilitySystemComponent;

/**
* Player state with RTS features, such as teams.
*/
UCLASS()
class ORDERSABILITIES_API ARTSPlayerState
	: public APlayerState
	, public IAbilitySystemInterface
{
	GENERATED_UCLASS_BODY()

public:
	/** Our ability system */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	URTSAbilitySystemComponent* AbilitySystem;

	/** Team this player belongs to. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnTeamChanged, Category = "Team")
	ARTSTeamInfo* Team;

	virtual void BeginPlay() override;

	/** Checks whether this player belong to the same team as the specified one. */
	UFUNCTION(BlueprintCallable)
	bool IsSameTeamAs(const ARTSPlayerState* Other) const;


	UFUNCTION()
	void OnTeamChanged();

	/** Event when this player changed team. */
	virtual void NotifyOnTeamChanged(ARTSTeamInfo* NewTeam);

	/** Event when this player changed team. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Team", meta = (DisplayName = "OnTeamChanged"))
	void ReceiveOnTeamChanged(ARTSTeamInfo* NewTeam);

	/** Team this player belongs to. */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnTeamChanged, Category = "Team")
	TArray<AActor*> OwnedActors;

	/** Event when the actor is owned by a different player. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSOwnedActorsChanged OnPartyActorChanged;

	void AddOwnedActor(AActor* Actor);
	void RemoveOwnedActor(AActor* Actor);

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
