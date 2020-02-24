#pragma once

#include "GameFramework/GameModeBase.h"

#include "RTSGameMode.generated.h"

class AController;
class ARTSPlayerAIController;
class ARTSPlayerController;
class ARTSPlayerStart;
class ARTSTeamInfo;

/**
* Game mode with RTS features, such as spawning initial units for each player.
*/
UCLASS()
class ORDERSABILITIES_API ARTSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	/** Actors to spawn for each player in the game. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
	TArray<TSubclassOf<AActor>> InitialActors;

    /** Relative locations of the actors to spawn for each player in the game, relative to their respetive start spot. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS")
    TArray<FVector> InitialActorPositions;

	/** Number of teams to create. */
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 NumTeams;

	/** Teams of the current match. */
	UPROPERTY(BlueprintReadOnly, Category = "Team")
	TArray<ARTSTeamInfo*> Teams;

	/** Class of TeamInfo to spawn. */
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	TSubclassOf<ARTSTeamInfo> TeamClass;

	ARTSGameMode(const FObjectInitializer& ObjectInitializer);

    virtual void BeginPlay() override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	ARTSPlayerStart* FindRTSPlayerStart(AController* Player);

	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;

	/** Spawns an actor of the specified type and transfers ownership to the specified player. */
	UFUNCTION(BlueprintCallable)
	virtual AActor* SpawnActorForPlayer(TSubclassOf<AActor> ActorClass, AController* ActorOwner, const FTransform& SpawnTransform);

    /** Sets the specified player as the owner of the passed actor. */
    UFUNCTION(BlueprintCallable)
    void TransferOwnership(AActor* Actor, AController* NewOwner);

	/** Actors to spawn for each player in the game. */
	UFUNCTION(BlueprintNativeEvent)
	void GetInitialActorsForPlayer(AController* Player, TArray<TSubclassOf<AActor>>& OutActors);
	
private:
    /** Number of AI players to spawn. */
    int32 NumAIPlayers;
};
