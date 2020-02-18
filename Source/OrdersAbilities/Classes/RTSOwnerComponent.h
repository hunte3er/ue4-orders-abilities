#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "GameFramework/PlayerState.h"

#include "RTSOwnerComponent.generated.h"


class AController;

class ARTSPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRTSOwnerComponentOwnerChangedSignature, APlayerState*, PreviousOwner, APlayerState*, NewOwner);

/**
* Specifies the owning player of the actor.
*/
UCLASS(meta = (BlueprintSpawnableComponent))
class ORDERSABILITIES_API URTSOwnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	URTSOwnerComponent(const FObjectInitializer& ObjectInitializer);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;


	/** Gets the player owning the actor. */
	UFUNCTION(BlueprintCallable)
	ARTSPlayerState* GetPlayerOwner() const;

	/** Sets the player owning the actor. */
	UFUNCTION(BlueprintCallable)
	void SetPlayerOwner(AController* Controller);


	/** Checks whether the actor belongs to the same team as the specified one. */
	UFUNCTION(BlueprintCallable)
	bool IsSameTeamAsActor(AActor* Other);

	/** Checks whether the player owning the actor belongs to the same team as the specified player. */
	UFUNCTION(BlueprintCallable)
	bool IsSameTeamAsController(AController* C);

	/** Event when the actor is owned by a different player. */
	UPROPERTY(BlueprintAssignable, Category = "RTS")
	FRTSOwnerComponentOwnerChangedSignature OnOwnerChanged;


private:
	/** Player owning this actor. */
	UPROPERTY(replicated)
	ARTSPlayerState* PlayerOwner;
};
