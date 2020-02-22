#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "Orders/RTSOrderData.h"

#include "RTSPlayerController.generated.h"


class USkeletalMesh;

class ARTSPlayerState;
class ARTSTeamInfo;
// class ARTSVisionInfo;

/**
 * Player controller with RTS features, such as selection and mouse camera movement.
 */
UCLASS()
class ORDERSABILITIES_API ARTSPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
    /** Movement speed of the camera when moved with keys or mouse, in cm/sec. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Camera", meta = (ClampMin = 0))
    float CameraSpeed;

    /** How fast to zoom the camera in and out, in cm/sec. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Camera")
    float CameraZoomSpeed;

	/** Maximum distance of the camera from the player pawn, in cm. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Camera", meta = (ClampMin = 0))
	float MinCameraDistance;

    /** Minimum distance of the camera from the player pawn, in cm. */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Camera", meta = (ClampMin = 0))
	float MaxCameraDistance;

    /** Distance from the screen border at which the mouse cursor causes the camera to move, in pixels. */
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RTS|Camera", meta = (ClampMin = 0))
	int32 CameraScrollThreshold;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RTS|Orders")
	TSoftClassPtr<URTSOrder> MoveOrder;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RTS|Orders")
	TSoftClassPtr<URTSOrder> AttackOrder;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "RTS|Orders")
	TSoftClassPtr<URTSOrder> StopOrder;


    ARTSPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	/** Gets the actor currently hovered by this player. */
	UFUNCTION(BlueprintCallable)
	AActor* GetHoveredActor();

	/** Gets the replicated state of this player. */
	UFUNCTION(BlueprintCallable)
	ARTSPlayerState* GetPlayerState();

	/** Gets the list of units currently selected by this player. */
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetSelectedActors();

	/** Casts a ray from the specified screen position and collects the results. */
	bool GetObjectsAtScreenPosition(FVector2D ScreenPosition, TArray<FHitResult>& OutHitResults);

	/** Casts a ray to find any objects at the specified world position. */
	bool GetObjectsAtWorldPosition(const FVector& WorldPositionXY, TArray<FHitResult>& OutHitResults);

	/** Gets the current selection frame, in screen space. */
	bool GetSelectionFrame(FIntRect& OutSelectionFrame);

	/** Gets the team this player belongs to. */
	UFUNCTION(BlueprintPure)
	ARTSTeamInfo* GetTeamInfo() const;

	/** Orders all selected units to attack the specified unit. */
	UFUNCTION(BlueprintCallable)
	bool IssueAttackOrder(AActor* Target, const FVector& TargetLocation);

	/** Orders all selected units to move to the specified location. */
	UFUNCTION(BlueprintCallable)
	bool IssueMoveOrder(AActor* TargetActor, const FVector& TargetLocation);

	/** Orders all selected units to stop all current actions. */
	UFUNCTION(BlueprintCallable)
	void IssueStopOrder();

	/** Selects the specified actors. */
	UFUNCTION(BlueprintCallable)
	void SelectActors(TArray<AActor*> Actors);

	/** Saves the current selection to the specified control group. */
	UFUNCTION(BlueprintCallable)
	void SaveControlGroup(int32 Index);

	UFUNCTION(BlueprintCallable) void SaveControlGroup0();
	UFUNCTION(BlueprintCallable) void SaveControlGroup1();
	UFUNCTION(BlueprintCallable) void SaveControlGroup2();
	UFUNCTION(BlueprintCallable) void SaveControlGroup3();
	UFUNCTION(BlueprintCallable) void SaveControlGroup4();
	UFUNCTION(BlueprintCallable) void SaveControlGroup5();
	UFUNCTION(BlueprintCallable) void SaveControlGroup6();
	UFUNCTION(BlueprintCallable) void SaveControlGroup7();
	UFUNCTION(BlueprintCallable) void SaveControlGroup8();
	UFUNCTION(BlueprintCallable) void SaveControlGroup9();

	/** Restores the selection saved in the specified control group. */
	UFUNCTION(BlueprintCallable)
	void LoadControlGroup(int32 Index);

	UFUNCTION(BlueprintCallable) void LoadControlGroup0();
	UFUNCTION(BlueprintCallable) void LoadControlGroup1();
	UFUNCTION(BlueprintCallable) void LoadControlGroup2();
	UFUNCTION(BlueprintCallable) void LoadControlGroup3();
	UFUNCTION(BlueprintCallable) void LoadControlGroup4();
	UFUNCTION(BlueprintCallable) void LoadControlGroup5();
	UFUNCTION(BlueprintCallable) void LoadControlGroup6();
	UFUNCTION(BlueprintCallable) void LoadControlGroup7();
	UFUNCTION(BlueprintCallable) void LoadControlGroup8();
	UFUNCTION(BlueprintCallable) void LoadControlGroup9();

	/** Whether the hotkey for showing all health bars is currently pressed, or not. */
	UFUNCTION(BlueprintCallable)
	bool IsHealthBarHotkeyPressed();

	/** Event when this player is now owning the specified actor. */
	virtual void NotifyOnActorOwnerChanged(AActor* Actor);

    /** Event when an error has occurred that can be presented to the user. */
    virtual void NotifyOnErrorOccurred(const FString& ErrorMessage);

	/** Event when an actor has received an attack order. */
	virtual void NotifyOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target);

    /** Event when an actor has received a move order. */
    virtual void NotifyOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);

	/** Event when an actor has received a stop order. */
	virtual void NotifyOnIssuedStopOrder(APawn* OrderedPawn);

    /** Event when the set of selected actors of this player has changed. */
    virtual void NotifyOnSelectionChanged(const TArray<AActor*>& Selection);

	/** Event when the team of this player has changed. */
	virtual void NotifyOnTeamChanged(ARTSTeamInfo* NewTeam);

	/** Event when vision info for this player has been replicated and is available. */
	// virtual void NotifyOnVisionInfoAvailable(ARTSVisionInfo* VisionInfo);

	/** Event when this player is now owning the specified actor. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Ownership", meta = (DisplayName = "OnActorOwnerChanged"))
	void ReceiveOnActorOwnerChanged(AActor* Actor);

    /** Event when an error has occurred that can be presented to the user. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Error", meta = (DisplayName = "OnErrorOccurred"))
    void ReceiveOnErrorOccurred(const FString& ErrorMessage);

	/** Event when an actor has received an attack order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedAttackOrder"))
	void ReceiveOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target);

    /** Event when an actor has received a move order. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedMoveOrder"))
    void ReceiveOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation);

	/** Event when an actor has received a stop order. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Orders", meta = (DisplayName = "OnIssuedStopOrder"))
	void ReceiveOnIssuedStopOrder(APawn* OrderedPawn);

    /** Event when the set of selected actors of this player has changed. */
    UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Selection", meta = (DisplayName = "OnSelectionChanged"))
    void ReceiveOnSelectionChanged(const TArray<AActor*>& Selection);

	/** Event when the team of this player has changed. */
	UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Team", meta = (DisplayName = "OnTeamChanged"))
	void ReceiveOnTeamChanged(ARTSTeamInfo* NewTeam);

	// /** Event when vision info for this player has been replicated and is available. */
	// UFUNCTION(BlueprintImplementableEvent, Category = "RTS|Vision", meta = (DisplayName = "OnVisionInfoAvailable"))
	// void ReceiveOnVisionInfoAvailable(ARTSVisionInfo* VisionInfo);

	bool bHasPendingOrder;
	FRTSOrderData PendingOrder;
	FRTSOrderPreviewData PendingOrderPreviewData;
	AActor* PendingOrderPreviewActor;

	UFUNCTION(BlueprintCallable)
	bool IsPendingOrder(TSoftClassPtr<URTSOrder> InOrderType, int32 InIndex) const;

	UFUNCTION(BlueprintCallable)
	void SetPendingOrder(TSoftClassPtr<URTSOrder> InOrderType, int32 InIndex);

	UFUNCTION(BlueprintCallable)
	void RemovePendingOrder();

	UFUNCTION(BlueprintCallable)
	bool DoesControllerOwnActor(AActor* Actor);
	
protected:
    virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
    /** Last horizontal axis input applied to camera movement. */
    float CameraLeftRightAxisValue;

    /** Last vertical axis input applied to camera movement. */
    float CameraUpDownAxisValue;

	/** Last zoom axis input applied to camera movement. */
	float CameraZoomAxisValue;

	/** Saved selections of this player. */
	TArray<TArray<AActor*>> ControlGroups;

	/** Actor currently hovered by this player. */
	AActor* HoveredActor;

	/** World position currently hovered by this player. */
	FVector HoveredWorldPosition;

    /** Actors selected by this player. */
    TArray<AActor*> SelectedActors;

	/** Whether we're currently creating a selection frame by dragging the mouse. */
	bool bCreatingSelectionFrame;

	/** Mouse position on screen when creating the selection frame started. */
	FVector2D SelectionFrameMouseStartPosition;

	/** Whether the hotkey for showing all health bars is currently pressed, or not. */
	bool bHealthBarHotkeyPressed;

	/** Whether the hotkey for showing all production progress bars is currently pressed, or not. */
	bool bProductionProgressBarHotkeyPressed;

	/** Whether to add clicked units to the current selection. */
	bool bAddSelectionHotkeyPressed;

	/** Whether to add clicked units to the current selection, if they're not already selected, and deselect them otherwise. */
	bool bToggleSelectionHotkeyPressed;

    /** Time to wait before playing the next selection sound, in seconds. */
    float SelectionSoundCooldownRemaining;

    /** Casts a ray from the current mouse position and collects the results. */
    bool GetObjectsAtPointerPosition(TArray<FHitResult>& OutHitResults);
	
	/** Casts a box from the current selection frame and collects the results. */
	bool GetObjectsInSelectionFrame(TArray<FHitResult>& OutHitResults);

	/** Traces all relevant objects using the specified ray. */
	bool TraceObjects(const FVector& WorldOrigin, const FVector& WorldDirection, TArray<FHitResult>& OutHitResults);

	/** Checks whether the specified actor is valid and selectable. */
	bool IsSelectableActor(AActor* Actor);

    /** Automatically issues the most reasonable order for the current pointer position. */
    UFUNCTION()
    void IssueOrder();

	/** Automatically issues the most reasonable order for the specified targets. */
	void IssueOrderTargetingObjects(TArray<FHitResult>& HitResults);

	bool IssuePendingOrder(TArray<FHitResult>& HitResults);

	/** Orders the passed unit to attack the specified unit. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueAttackOrder(APawn* OrderedPawn, AActor* Target, const FVector& TargetLocation, bool bEnqueueOrder);
	
	/** Orders the passed unit to move to the specified location. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueMoveOrder(APawn* OrderedPawn, AActor* Target, const FVector& TargetLocation, bool bEnqueueOrder);

	/** Orders the passed unit to stop all current actions. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssueStopOrder(APawn* OrderedPawn, bool bEnqueueOrder);

	/** Orders the passed unit to stop all current actions. */
	UFUNCTION(Reliable, Server, WithValidation)
	void ServerIssuePendingOrder(APawn* OrderedPawn, const FRTSOrderData& InPendingOrder, bool bEnqueueOrder);

    /** Applies horizontal axis input to camera movement. */
    void MoveCameraLeftRight(float Value);

    /** Applies vertical axis input to camera movement. */
    void MoveCameraUpDown(float Value);

    /** Applies zoom input to camera movement. */
    void ZoomCamera(float Value);

    /** Remembers the current mouse position for multi-selection, finished by FinishSelectActors. */
    UFUNCTION()
    void StartSelectActors();

	/** Selects all selectable actors within the created selection frame, started by StartSelectActors. */
	UFUNCTION()
	void FinishSelectActors();

	/** Force showing all health bars. */
	UFUNCTION()
	void StartShowingHealthBars();

	/** Stop showing all health bars. */
	UFUNCTION()
	void StopShowingHealthBars();

	/** Start adding clicked units to the current selection. */
	UFUNCTION()
	void StartAddSelection();

	/** Stop adding clicked units to the current selection. */
	UFUNCTION()
	void StopAddSelection();

	/** Start adding clicked units to the current selection, if they're not already selected, and deselecting them otherwise. */
	UFUNCTION()
	void StartToggleSelection();

	/** Stop adding clicked units to the current selection, if they're not already selected, and deselecting them otherwise. */
	UFUNCTION()
	void StopToggleSelection();
};
