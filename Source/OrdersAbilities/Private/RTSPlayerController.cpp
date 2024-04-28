
#include "RTSPlayerController.h"

#include "OrdersAbilities/OrdersAbilities.h"

#include "EngineUtils.h"
#include "Landscape.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Engine/Engine.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

#include "Orders/RTSOrderHelper.h"
#include "RTSGameMode.h"
#include "RTSOwnerComponent.h"
#include "RTSPlayerState.h"
#include "RTSSelectableComponent.h"
#include "RTSTeamInfo.h"
#include "RTSUtilities.h"
#include "Orders/RTSOrderHelper.h"
#include "Orders/OrdersAbilitiesAIController.h"
#include "GameFramework/SpringArmComponent.h"


ARTSPlayerController::ARTSPlayerController()
{
  	// Set reasonable default values.
    CameraSpeed = 1000.0f;
    CameraZoomSpeed = 4000.0f;

    MinCameraDistance = 500.0f;
	MaxCameraDistance = 2500.0f;

	CameraScrollThreshold = 20.0f;

	bHasPendingOrder = false;
}

bool ARTSPlayerController::IsPendingOrder(TSoftClassPtr<URTSOrder> InOrderType, int32 InIndex) const
{
	if (!bHasPendingOrder)
		return false;

	return PendingOrder.OrderType == InOrderType && PendingOrder.Index == InIndex;
}

void ARTSPlayerController::SetPendingOrder(TSoftClassPtr<URTSOrder> InOrderType, int32 InIndex)
{
	if (SelectedActors.Num() == 0)
	{
		return;
	}
	
	AActor* ActorWhoCanObeyOrder = nullptr;

	for (AActor* Actor : SelectedActors)
	{ 
		if (URTSOrderHelper::CanObeyOrder(InOrderType, Actor, InIndex) && DoesControllerOwnActor(Actor))
		{
			ActorWhoCanObeyOrder = Actor;
			break;
		}
	}
	
	if (ActorWhoCanObeyOrder == nullptr)
	{
		return;
	}

	UE_LOG(LogRTS, Log, TEXT("Set pending order %s | %d"), *InOrderType.ToString(), InIndex);

	// If the order no target type, just activate the ability
	auto Order = URTSOrderHelper::GetDefaultOrder(InOrderType);
	if (Order->IsTargetTypeFlagChecked(ActorWhoCanObeyOrder, InIndex, ERTSTargetTypeFlags::NONE))
	{
		auto OrderData = FRTSOrderData(InOrderType, InIndex);
		
		for (auto SelectedActor : SelectedActors)
		{
			if (!DoesControllerOwnActor(SelectedActor))
			{
				continue;
			}
			
			if (auto SelectedPawn = Cast<APawn>(SelectedActor))
			{
				if (URTSOrderHelper::CheckOrder(SelectedPawn, OrderData).IsEmpty())
				{
					ServerIssuePendingOrder(SelectedPawn, OrderData, IsInputKeyDown(EKeys::LeftShift));
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Failed to issue order with target type NONE"));
				}
			}
		}

		return;
	}
	
	PendingOrder = FRTSOrderData(InOrderType, InIndex);
	
	bHasPendingOrder = true;
	PendingOrderPreviewData = URTSOrderHelper::GetOrderPreviewData(InOrderType, SelectedActors[0], InIndex);
	if (PendingOrderPreviewData.GetOrderPreviewClass() != nullptr)
	{
		if (IsValid(PendingOrderPreviewActor))
		{
			PendingOrderPreviewActor->Destroy();
		}
		
		PendingOrderPreviewActor = GetWorld()->SpawnActor(PendingOrderPreviewData.GetOrderPreviewClass());

		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Preview data not valid!"));
	}
}

void ARTSPlayerController::RemovePendingOrder()
{
	if (IsValid(PendingOrderPreviewActor))
	{
		PendingOrderPreviewActor->Destroy();
		PendingOrderPreviewActor = nullptr;
	}

	PendingOrder = FRTSOrderData();

	bHasPendingOrder = false;
}

bool ARTSPlayerController::IssuePendingOrder(TArray<FHitResult>& HitResults)
{
	if (!bHasPendingOrder)
	{
		return false;
	}

	// Check if there's anybody to receive the order.
	if (SelectedActors.Num() == 0)
	{
		return false;
	}
	
	// Check hit actors.
	for (auto& HitResult : HitResults)
	{
		if (HitResult.GetActor() != nullptr)
		{
			// Issue attack order.
			FRTSOrderData OrderWithTarget = FRTSOrderData(PendingOrder.OrderType, PendingOrder.Index, HitResult.GetActor());
			FRTSOrderData OrderWithLocation = FRTSOrderData(PendingOrder.OrderType, PendingOrder.Index, FVector2D(HitResult.Location));

			bool bSuccess = false;
			for (auto SelectedActor : GetSelectedActors())
			{
				APawn* SelectedPawn = Cast<APawn>(SelectedActor);
				if (!SelectedPawn)
				{
					continue;
				}

				if (!DoesControllerOwnActor(SelectedPawn))
				{
					continue;
				}
				
				if (URTSOrderHelper::CheckOrder(SelectedPawn, OrderWithTarget).IsEmpty())
				{
					ServerIssuePendingOrder(SelectedPawn, OrderWithTarget, IsInputKeyDown(EKeys::LeftShift));
					bSuccess = true;
				}
				else if (URTSOrderHelper::CheckOrder(SelectedPawn, OrderWithLocation).IsEmpty())
				{
					ServerIssuePendingOrder(SelectedPawn, OrderWithLocation, IsInputKeyDown(EKeys::LeftShift));
					bSuccess = true;
				}
			}

			if (bSuccess)
			{
				RemovePendingOrder();
				return true;
			}
			else
			{
				UE_LOG(LogRTS, Log, TEXT("Failed to issue pending order"));
			}
		}
	}

	return false;
}

bool ARTSPlayerController::DoesControllerOwnActor(AActor* Actor)
{
	// // Verify owner to prevent cheating.
	// URTSOwnerComponent* OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();
	// if (OwnerComponent)
	// 	return GetPlayerState() == OwnerComponent->GetPlayerOwner();

	// Check if same owner is the same player
	URTSOwnerComponent* OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();
	if (!OwnerComponent)
	{
		return false;
	}

	ARTSPlayerState* ActorOwner = OwnerComponent->GetPlayerOwner();
	if (!OwnerComponent->GetPlayerOwner())
	{
		return false;
	}

	return ActorOwner->GetPlayerId() == GetPlayerState()->GetPlayerId();
}

void ARTSPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enable mouse input.
	APlayerController::bShowMouseCursor = true;
	APlayerController::bEnableClickEvents = true;
	APlayerController::bEnableMouseOverEvents = true;

	// Bind actions.
	InputComponent->BindAction(TEXT("Select"), IE_Pressed, this, &ARTSPlayerController::StartSelectActors);
	InputComponent->BindAction(TEXT("Select"), IE_Released, this, &ARTSPlayerController::FinishSelectActors);
	InputComponent->BindAction(TEXT("AddSelection"), IE_Pressed, this, &ARTSPlayerController::StartAddSelection);
	InputComponent->BindAction(TEXT("AddSelection"), IE_Released, this, &ARTSPlayerController::StopAddSelection);
	InputComponent->BindAction(TEXT("ToggleSelection"), IE_Pressed, this, &ARTSPlayerController::StartToggleSelection);
	InputComponent->BindAction(TEXT("ToggleSelection"), IE_Released, this, &ARTSPlayerController::StopToggleSelection);
	
	InputComponent->BindAction(TEXT("IssueOrder"), IE_Released, this, &ARTSPlayerController::IssueOrder);
	InputComponent->BindAction(TEXT("IssueStopOrder"), IE_Released, this, &ARTSPlayerController::IssueStopOrder);

	InputComponent->BindAction(TEXT("SaveControlGroup0"), IE_Released, this, &ARTSPlayerController::SaveControlGroup0);
	InputComponent->BindAction(TEXT("SaveControlGroup1"), IE_Released, this, &ARTSPlayerController::SaveControlGroup1);
	InputComponent->BindAction(TEXT("SaveControlGroup2"), IE_Released, this, &ARTSPlayerController::SaveControlGroup2);
	InputComponent->BindAction(TEXT("SaveControlGroup3"), IE_Released, this, &ARTSPlayerController::SaveControlGroup3);
	InputComponent->BindAction(TEXT("SaveControlGroup4"), IE_Released, this, &ARTSPlayerController::SaveControlGroup4);
	InputComponent->BindAction(TEXT("SaveControlGroup5"), IE_Released, this, &ARTSPlayerController::SaveControlGroup5);
	InputComponent->BindAction(TEXT("SaveControlGroup6"), IE_Released, this, &ARTSPlayerController::SaveControlGroup6);
	InputComponent->BindAction(TEXT("SaveControlGroup7"), IE_Released, this, &ARTSPlayerController::SaveControlGroup7);
	InputComponent->BindAction(TEXT("SaveControlGroup8"), IE_Released, this, &ARTSPlayerController::SaveControlGroup8);
	InputComponent->BindAction(TEXT("SaveControlGroup9"), IE_Released, this, &ARTSPlayerController::SaveControlGroup9);

	InputComponent->BindAction(TEXT("LoadControlGroup0"), IE_Released, this, &ARTSPlayerController::LoadControlGroup0);
	InputComponent->BindAction(TEXT("LoadControlGroup1"), IE_Released, this, &ARTSPlayerController::LoadControlGroup1);
	InputComponent->BindAction(TEXT("LoadControlGroup2"), IE_Released, this, &ARTSPlayerController::LoadControlGroup2);
	InputComponent->BindAction(TEXT("LoadControlGroup3"), IE_Released, this, &ARTSPlayerController::LoadControlGroup3);
	InputComponent->BindAction(TEXT("LoadControlGroup4"), IE_Released, this, &ARTSPlayerController::LoadControlGroup4);
	InputComponent->BindAction(TEXT("LoadControlGroup5"), IE_Released, this, &ARTSPlayerController::LoadControlGroup5);
	InputComponent->BindAction(TEXT("LoadControlGroup6"), IE_Released, this, &ARTSPlayerController::LoadControlGroup6);
	InputComponent->BindAction(TEXT("LoadControlGroup7"), IE_Released, this, &ARTSPlayerController::LoadControlGroup7);
	InputComponent->BindAction(TEXT("LoadControlGroup8"), IE_Released, this, &ARTSPlayerController::LoadControlGroup8);
	InputComponent->BindAction(TEXT("LoadControlGroup9"), IE_Released, this, &ARTSPlayerController::LoadControlGroup9);

	InputComponent->BindAxis(TEXT("MoveCameraLeftRight"), this, &ARTSPlayerController::MoveCameraLeftRight);
	InputComponent->BindAxis(TEXT("MoveCameraUpDown"), this, &ARTSPlayerController::MoveCameraUpDown);
    InputComponent->BindAxis(TEXT("ZoomCamera"), this, &ARTSPlayerController::ZoomCamera);

	InputComponent->BindAction(TEXT("ShowHealthBars"), IE_Pressed, this, &ARTSPlayerController::StartShowingHealthBars);
	InputComponent->BindAction(TEXT("ShowHealthBars"), IE_Released, this, &ARTSPlayerController::StopShowingHealthBars);
	// InputComponent->BindAction(TEXT("ShowProductionProgressBars"), IE_Pressed, this, &ARTSPlayerController::StartShowingProductionProgressBars);
	// InputComponent->BindAction(TEXT("ShowProductionProgressBars"), IE_Released, this, &ARTSPlayerController::StopShowingProductionProgressBars);

	// InputComponent->BindAction(TEXT("BeginDefaultBuildingPlacement"), IE_Released, this, &ARTSPlayerController::BeginDefaultBuildingPlacement);
	// InputComponent->BindAction(TEXT("ConfirmBuildingPlacement"), IE_Released, this, &ARTSPlayerController::ConfirmBuildingPlacement);
	// InputComponent->BindAction(TEXT("CancelBuildingPlacement"), IE_Released, this, &ARTSPlayerController::CancelBuildingPlacement);
	//
	// InputComponent->BindAction(TEXT("CancelConstruction"), IE_Released, this, &ARTSPlayerController::CancelConstruction);

	// InputComponent->BindAction(TEXT("StartDefaultProduction"), IE_Released, this, &ARTSPlayerController::StartDefaultProduction);
	// InputComponent->BindAction(TEXT("CancelProduction"), IE_Released, this, &ARTSPlayerController::CancelProduction);

	// Setup control groups.
	ControlGroups.SetNum(10);
}

void ARTSPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

AActor* ARTSPlayerController::GetHoveredActor()
{
	return HoveredActor;
}

ARTSPlayerState* ARTSPlayerController::GetPlayerState()
{
	return Cast<ARTSPlayerState>(PlayerState);
}

TArray<AActor*> ARTSPlayerController::GetSelectedActors()
{
	return SelectedActors;
}

bool ARTSPlayerController::GetObjectsAtScreenPosition(FVector2D ScreenPosition, TArray<FHitResult>& OutHitResults)
{
	// Get ray.
	FVector WorldOrigin;
	FVector WorldDirection;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ScreenPosition, WorldOrigin, WorldDirection))
	{
		return false;
	}

	// Cast ray.
	return TraceObjects(WorldOrigin, WorldDirection, OutHitResults);
}

bool ARTSPlayerController::GetObjectsAtWorldPosition(const FVector& WorldPositionXY, TArray<FHitResult>& OutHitResults)
{
	// Get ray.
	FVector WorldOrigin = FVector(WorldPositionXY.X, WorldPositionXY.Y, HitResultTraceDistance / 2);
	FVector WorldDirection = -FVector::UpVector;

	// Cast ray.
	return TraceObjects(WorldOrigin, WorldDirection, OutHitResults);
}

bool ARTSPlayerController::GetSelectionFrame(FIntRect& OutSelectionFrame)
{
	if (!bCreatingSelectionFrame)
	{
		return false;
	}

	// Get mouse input.
	float MouseX;
	float MouseY;

	if (!GetMousePosition(MouseX, MouseY))
	{
		return false;
	}

    float MinX = FMath::Min(SelectionFrameMouseStartPosition.X, MouseX);
    float MaxX = FMath::Max(SelectionFrameMouseStartPosition.X, MouseX);
    float MinY = FMath::Min(SelectionFrameMouseStartPosition.Y, MouseY);
    float MaxY = FMath::Max(SelectionFrameMouseStartPosition.Y, MouseY);

	OutSelectionFrame = FIntRect(FIntPoint(MinX, MinY), FIntPoint(MaxX, MaxY));

	return true;
}

ARTSTeamInfo* ARTSPlayerController::GetTeamInfo() const
{
	ARTSPlayerState* CurrentPlayerState = Cast<ARTSPlayerState>(PlayerState);

	if (CurrentPlayerState)
	{
		return CurrentPlayerState->Team;
	}
	
	return nullptr;
}

bool ARTSPlayerController::GetObjectsAtPointerPosition(TArray<FHitResult>& OutHitResults)
{
    // Get local player viewport.
    ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);

    if (!LocalPlayer || !LocalPlayer->ViewportClient)
    {
        return false;
    }

    // Get mouse position.
    FVector2D MousePosition;
    if (!LocalPlayer->ViewportClient->GetMousePosition(MousePosition))
    {
        return false;
    }

	return GetObjectsAtScreenPosition(MousePosition, OutHitResults);
}

bool ARTSPlayerController::GetObjectsInSelectionFrame(TArray<FHitResult>& HitResults)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	// Get selection frame.
	FIntRect SelectionFrame;
	
	if (!GetSelectionFrame(SelectionFrame))
	{
		return false;
	}

	if (SelectionFrame.Area() < 10)
	{
		// Selection frame too small - just consider left-click.
		return GetObjectsAtPointerPosition(HitResults);
	}

	// Iterate all actors.
	HitResults.Reset();

	for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		AActor* Actor = *ActorIt;

		FVector2D ActorScreenPosition;

		if (UGameplayStatics::ProjectWorldToScreen(this, Actor->GetActorLocation(), ActorScreenPosition))
		{
			if (SelectionFrame.Contains(FIntPoint(ActorScreenPosition.X, ActorScreenPosition.Y)))
			{
				FHitResult HitResult(Actor, nullptr, Actor->GetActorLocation(), FVector());
				HitResults.Add(HitResult);
			}
		}
	}

	return HitResults.Num() > 0;
}

bool ARTSPlayerController::TraceObjects(const FVector& WorldOrigin, const FVector& WorldDirection, TArray<FHitResult>& OutHitResults)
{
	UWorld* World = GetWorld();

	if (!World)
	{
		return false;
	}

	FCollisionObjectQueryParams Params(FCollisionObjectQueryParams::InitType::AllObjects);

	return World->LineTraceMultiByObjectType(
		OutHitResults,
		WorldOrigin,
		WorldOrigin + WorldDirection * HitResultTraceDistance,
		Params);
}

bool ARTSPlayerController::IsSelectableActor(AActor* Actor)
{
	// Check if valid.
	if (!IsValid(Actor))
	{
		return false;
	}

	// Check if selectable.
	auto SelectableComponent = Actor->FindComponentByClass<URTSSelectableComponent>();

	if (!SelectableComponent)
	{
		return false;
	}

	return true;
	
	// // Check if same owner is the same player
	// URTSOwnerComponent* OwnerComponent = Actor->FindComponentByClass<URTSOwnerComponent>();
	// if (!OwnerComponent)
	// {
	// 	return false;
	// }
	//
	// ARTSPlayerState* ActorOwner = OwnerComponent->GetPlayerOwner();
	// if (!OwnerComponent->GetPlayerOwner())
	// {
	// 	return false;
	// }
	//
	// return ActorOwner->PlayerId == GetPlayerState()->PlayerId;
}

void ARTSPlayerController::IssueOrder()
{
    // Get objects at pointer position.
    TArray<FHitResult> HitResults;

	RemovePendingOrder();
	
    if (!GetObjectsAtPointerPosition(HitResults))
    {
        return;
    }

	IssueOrderTargetingObjects(HitResults);
}

void ARTSPlayerController::IssueOrderTargetingObjects(TArray<FHitResult>& HitResults)
{
	// Check if there's anybody to receive the order.
	if (SelectedActors.Num() == 0)
	{
		return;
	}

	// Get target location.
	for (auto& HitResult : HitResults)
	{
		if (HitResult.GetActor() != nullptr)
		{
			// Issue attack order.
			if (IssueAttackOrder(HitResult.GetActor(), HitResult.Location))
			{
				return;
			}
			
			// // Issue gather order.
			// if (IssueGatherOrder(HitResult.GetActor()))
			// {
			// 	return;
			// }
			//
			// // Issue construct order.
			// if (IssueContinueConstructionOrder(HitResult.GetActor()))
			// {
			// 	return;
			// }

            // ALandscape* Landscape = Cast<ALandscape>(HitResult.GetActor());
            //
            // if (Landscape != nullptr)
            // {
            // // Issue move order.
            // IssueMoveOrder(HitResult.Location);
            // return;
            // }

			if (IssueMoveOrder(HitResult.GetActor(), HitResult.Location))
			{
				return;
			}
		}

		// Issue move order.
		if (IssueMoveOrder(nullptr, HitResult.Location))
		{
			return;
		}
	}
}

bool ARTSPlayerController::IssueAttackOrder(AActor* Target, const FVector& TargetLocation)
{
	if (!Target)
	{
		return false;
	}

	// Verify target.
	auto TargetOwnerComponent = Target->FindComponentByClass<URTSOwnerComponent>();
	if (!TargetOwnerComponent)
	{
		return false;
	}

	ARTSTeamInfo* MyTeam = GetPlayerState()->Team;

	// Issue attack orders.
	bool bSuccess = false;

	for (auto SelectedActor : SelectedActors)
	{
		APawn* SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}
		
		if (!DoesControllerOwnActor(SelectedPawn))
		{
			continue;
		}

		// if (TargetOwnerComponent->IsSameTeamAsActor(SelectedActor))
		// {
		// 	continue;
		// }

		FRTSOrderData AttackOrderData(AttackOrder, Target, FVector2D(TargetLocation));
		if (!URTSOrderHelper::CheckOrder(SelectedActor, AttackOrderData).IsEmpty())
		{
			continue;
		}
		
		// Send attack order to server.
		ServerIssueAttackOrder(SelectedPawn, Target, TargetLocation, IsInputKeyDown(EKeys::LeftShift));

        if (IsNetMode(NM_Client))
        {
            UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to attack %s."), *SelectedActor->GetName(), *Target->GetName());

            // Notify listeners.
            NotifyOnIssuedAttackOrder(SelectedPawn, Target);
        }

		bSuccess = true;
	}

	return bSuccess;
}

void ARTSPlayerController::ServerIssueAttackOrder_Implementation(APawn* OrderedPawn, AActor* Target, const FVector& TargetLocation, bool bEnqueueOrder)
{
	if (bEnqueueOrder)
	{
		URTSOrderHelper::EnqueueOrder(OrderedPawn, FRTSOrderData(AttackOrder, Target, FVector2D(TargetLocation)));
	}
	else
	{
		URTSOrderHelper::IssueOrder(OrderedPawn, FRTSOrderData(AttackOrder, Target, FVector2D(TargetLocation)));
	}

	UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to attack %s."), *OrderedPawn->GetName(), *Target->GetName());

	// Notify listeners.
	NotifyOnIssuedAttackOrder(OrderedPawn, Target);
}

bool ARTSPlayerController::ServerIssueAttackOrder_Validate(APawn* OrderedPawn, AActor* Target, const FVector& TargetLocation, bool bEnqueueOrder)
{
	return DoesControllerOwnActor(OrderedPawn);
}

bool ARTSPlayerController::IssueMoveOrder(AActor* TargetActor, const FVector& TargetLocation)
{
    // Issue move orders.
	bool bSuccess = false;

	FRTSOrderTargetData TargetData;
	TargetData.Location = FVector2D(TargetLocation);

	TArray<FVector2D> TargetLocations;
	URTSOrderHelper::CreateIndividualTargetLocations(MoveOrder, SelectedActors, TargetData, TargetLocations);

	FRTSOrderData OrderWithTarget = FRTSOrderData(MoveOrder, TargetActor);
	FRTSOrderData OrderWithLocation = FRTSOrderData(MoveOrder, FVector2D(TargetLocation));
	
    for (int i = 0; i < SelectedActors.Num(); ++i)
    {
		AActor* SelectedActor = SelectedActors[i];
		OrderWithLocation.Location = TargetLocations[i];
    	
        // Verify pawn and owner.
        auto SelectedPawn = Cast<APawn>(SelectedActor);

        if (!SelectedPawn)
        {
            continue;
        }

		if (!DoesControllerOwnActor(SelectedPawn))
		{
			continue;
		}
    	
		FRTSOrderData* OrderToUse = nullptr;
		if (URTSOrderHelper::CheckOrder(SelectedActor, OrderWithTarget).IsEmpty())
		{
			OrderToUse = &OrderWithTarget;
		}
		else if (URTSOrderHelper::CheckOrder(SelectedActor, OrderWithLocation).IsEmpty())
		{
			OrderToUse = &OrderWithLocation;
		}
		else
		{
			continue;
		}
    	
		// Send move order to server.
		ServerIssueMoveOrder(SelectedPawn, OrderToUse->Target, FVector(OrderToUse->Location, 0.0f), IsInputKeyDown(EKeys::LeftShift));

        if (IsNetMode(NM_Client))
        {
            UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to move to %s."), *SelectedActor->GetName(), *TargetLocation.ToString());

            // Notify listeners.
            NotifyOnIssuedMoveOrder(SelectedPawn, FVector(OrderToUse->Location, 0.0f));
        }

		bSuccess = true;
    }

	return bSuccess;
}

void ARTSPlayerController::ServerIssueMoveOrder_Implementation(APawn* OrderedPawn, AActor* Target, const FVector& TargetLocation, bool bEnqueueOrder)
{
	FRTSOrderData MoveOrderData(MoveOrder, Target, FVector2D(TargetLocation));

	if (bEnqueueOrder)
	{
		URTSOrderHelper::EnqueueOrder(OrderedPawn, MoveOrderData);
	}
	else
	{
		URTSOrderHelper::IssueOrder(OrderedPawn, MoveOrderData);
	}

	// Issue move order.
	if (Target != nullptr)
	{
		UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to move to %s."), *OrderedPawn->GetName(), *Target->GetName());
	}
	else
	{
		UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to move to %s."), *OrderedPawn->GetName(), *TargetLocation.ToString());
	}
		

	// Notify listeners.
	NotifyOnIssuedMoveOrder(OrderedPawn, TargetLocation);
}

bool ARTSPlayerController::ServerIssueMoveOrder_Validate(APawn* OrderedPawn, AActor* Target, const FVector& TargetLocation, bool bEnqueueOrder)
{
	return DoesControllerOwnActor(OrderedPawn);
}

void ARTSPlayerController::IssueStopOrder()
{
	// Issue stop orders.
	for (auto SelectedActor : SelectedActors)
	{
		// Verify pawn and owner.
		auto SelectedPawn = Cast<APawn>(SelectedActor);

		if (!SelectedPawn)
		{
			continue;
		}
		
		if (!DoesControllerOwnActor(SelectedPawn))
		{
			continue;
		}

		// Send stop order to server.
		ServerIssueStopOrder(SelectedPawn, IsInputKeyDown(EKeys::LeftShift));

        if (IsNetMode(NM_Client))
        {
            UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to stop."), *SelectedActor->GetName());

            // Notify listeners.
            NotifyOnIssuedStopOrder(SelectedPawn);
        }
	}
}

void ARTSPlayerController::ServerIssueStopOrder_Implementation(APawn* OrderedPawn, bool bEnqueueOrder)
{
	if (bEnqueueOrder)
	{
		URTSOrderHelper::EnqueueOrder(OrderedPawn, StopOrder);
	}
	else
	{
		URTSOrderHelper::IssueOrder(OrderedPawn, StopOrder);
	}
	
	UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to stop."), *OrderedPawn->GetName());

	// Notify listeners.
	NotifyOnIssuedStopOrder(OrderedPawn);
}

bool ARTSPlayerController::ServerIssueStopOrder_Validate(APawn* OrderedPawn, bool bEnqueueOrder)
{
	return DoesControllerOwnActor(OrderedPawn);
}

void ARTSPlayerController::ServerIssuePendingOrder_Implementation(APawn* OrderedPawn, const FRTSOrderData& InPendingOrder, bool bEnqueueOrder)
{	
	if (bEnqueueOrder)
	{
		URTSOrderHelper::EnqueueOrder(OrderedPawn, InPendingOrder);
	}
	else
	{
		URTSOrderHelper::IssueOrder(OrderedPawn, InPendingOrder);
	}

	UE_LOG(LogRTS, Log, TEXT("Ordered actor %s to obey pending order."), *OrderedPawn->GetName());

	// Notify listeners.
	// NotifyOnIssuedStopOrder(OrderedPawn);
}

bool ARTSPlayerController::ServerIssuePendingOrder_Validate(APawn* OrderedPawn, const FRTSOrderData& InPendingOrder, bool bEnqueueOrder)
{
	return DoesControllerOwnActor(OrderedPawn);
}

void ARTSPlayerController::SelectActors(TArray<AActor*> Actors)
{
	// Clear selection.
	for (AActor* SelectedActor : SelectedActors)
	{
		URTSSelectableComponent* SelectableComponent = SelectedActor->FindComponentByClass<URTSSelectableComponent>();

		if (SelectableComponent)
		{
			SelectableComponent->DeselectActor();
		}
	}

	// Apply new selection.
	SelectedActors = Actors;

	for (AActor* SelectedActor : SelectedActors)
	{
		URTSSelectableComponent* SelectableComponent = SelectedActor->FindComponentByClass<URTSSelectableComponent>();

		if (SelectableComponent)
		{
			SelectableComponent->SelectActor(this);

            // Play selection sound.
            if (SelectionSoundCooldownRemaining <= 0.0f && IsValid(SelectableComponent->SelectedSound))
            {
                UGameplayStatics::PlaySound2D(this, SelectableComponent->SelectedSound);
                SelectionSoundCooldownRemaining = SelectableComponent->SelectedSound->GetDuration();
            }
		}
	}

	// Notify listeners.
	NotifyOnSelectionChanged(SelectedActors);
}

void ARTSPlayerController::SaveControlGroup(int32 Index)
{
	if (Index < 0 || Index > 9)
	{
		return;
	}

	// Save control group.
	ControlGroups[Index] = SelectedActors;

	UE_LOG(LogRTS, Log, TEXT("Saved selection to control group %d."), Index);
}

void ARTSPlayerController::SaveControlGroup0() { SaveControlGroup(0); }
void ARTSPlayerController::SaveControlGroup1() { SaveControlGroup(1); }
void ARTSPlayerController::SaveControlGroup2() { SaveControlGroup(2); }
void ARTSPlayerController::SaveControlGroup3() { SaveControlGroup(3); }
void ARTSPlayerController::SaveControlGroup4() { SaveControlGroup(4); }
void ARTSPlayerController::SaveControlGroup5() { SaveControlGroup(5); }
void ARTSPlayerController::SaveControlGroup6() { SaveControlGroup(6); }
void ARTSPlayerController::SaveControlGroup7() { SaveControlGroup(7); }
void ARTSPlayerController::SaveControlGroup8() { SaveControlGroup(8); }
void ARTSPlayerController::SaveControlGroup9() { SaveControlGroup(9); }

void ARTSPlayerController::LoadControlGroup(int32 Index)
{
	if (Index < 0 || Index > 9)
	{
		return;
	}

	SelectActors(ControlGroups[Index]);

	UE_LOG(LogRTS, Log, TEXT("Loaded selection from control group %d."), Index);
}

void ARTSPlayerController::LoadControlGroup0() { LoadControlGroup(0); }
void ARTSPlayerController::LoadControlGroup1() { LoadControlGroup(1); }
void ARTSPlayerController::LoadControlGroup2() { LoadControlGroup(2); }
void ARTSPlayerController::LoadControlGroup3() { LoadControlGroup(3); }
void ARTSPlayerController::LoadControlGroup4() { LoadControlGroup(4); }
void ARTSPlayerController::LoadControlGroup5() { LoadControlGroup(5); }
void ARTSPlayerController::LoadControlGroup6() { LoadControlGroup(6); }
void ARTSPlayerController::LoadControlGroup7() { LoadControlGroup(7); }
void ARTSPlayerController::LoadControlGroup8() { LoadControlGroup(8); }
void ARTSPlayerController::LoadControlGroup9() { LoadControlGroup(9); }

bool ARTSPlayerController::IsHealthBarHotkeyPressed()
{
	return bHealthBarHotkeyPressed;
}

void ARTSPlayerController::StartSelectActors()
{
	// Get mouse input.
	float MouseX;
	float MouseY;

	TArray<FHitResult> HitResults;
	if (GetObjectsAtPointerPosition(HitResults))
	{
		if (IssuePendingOrder(HitResults))
		{
			return;
		}
	}
	
	if (GetMousePosition(MouseX, MouseY))
	{
		SelectionFrameMouseStartPosition = FVector2D(MouseX, MouseY);
		bCreatingSelectionFrame = true;
	}
}

void ARTSPlayerController::FinishSelectActors()
{
    // Get objects at pointer position.
    TArray<FHitResult> HitResults;
    
    if (!GetObjectsInSelectionFrame(HitResults))
    {
		bCreatingSelectionFrame = false;
        return;
    }

    // Check results.
	TArray<AActor*> ActorsToSelect;

	if (bAddSelectionHotkeyPressed || bToggleSelectionHotkeyPressed)
	{
		ActorsToSelect = SelectedActors;
	}

    for (auto& HitResult : HitResults)
    {
		if (!IsSelectableActor(HitResult.GetActor()))
		{
			continue;
		}

		// Check how to apply selection.
		if (bToggleSelectionHotkeyPressed)
		{
			if (SelectedActors.Contains(HitResult.GetActor()))
			{
				// Deselect actor.
				ActorsToSelect.Remove(HitResult.GetActor());

				UE_LOG(LogRTS, Log, TEXT("Deselected actor %s."), *HitResult.GetActor()->GetName());
			}
			else if (!ActorsToSelect.Contains(HitResult.GetActor()))
			{
				// Select actor.
				ActorsToSelect.Add(HitResult.GetActor());

				UE_LOG(LogRTS, Log, TEXT("Selected actor %s."), *HitResult.GetActor()->GetName());
			}
		}
		else
		{
			if (ActorsToSelect.Contains(HitResult.GetActor()))
			{
				continue;
			}

			// Select actor.
			ActorsToSelect.Add(HitResult.GetActor());

			UE_LOG(LogRTS, Log, TEXT("Selected actor %s."), *HitResult.GetActor()->GetName());
		}
    }

	SelectActors(ActorsToSelect);

	bCreatingSelectionFrame = false;
}

void ARTSPlayerController::StartShowingHealthBars()
{
	bHealthBarHotkeyPressed = true;
}

void ARTSPlayerController::StopShowingHealthBars()
{
	bHealthBarHotkeyPressed = false;
}

void ARTSPlayerController::StartAddSelection()
{
	bAddSelectionHotkeyPressed = true;
}

void ARTSPlayerController::StopAddSelection()
{
	bAddSelectionHotkeyPressed = false;
}

void ARTSPlayerController::StartToggleSelection()
{
	bToggleSelectionHotkeyPressed = true;
}

void ARTSPlayerController::StopToggleSelection()
{
	bToggleSelectionHotkeyPressed = false;
}

void ARTSPlayerController::MoveCameraLeftRight(float Value)
{
    CameraLeftRightAxisValue = Value;
}

void ARTSPlayerController::MoveCameraUpDown(float Value)
{
    CameraUpDownAxisValue = Value;
}

void ARTSPlayerController::ZoomCamera(float Value)
{
    CameraZoomAxisValue = Value;
}

void ARTSPlayerController::NotifyOnActorOwnerChanged(AActor* Actor)
{
	ReceiveOnActorOwnerChanged(Actor);
}

void ARTSPlayerController::NotifyOnErrorOccurred(const FString& ErrorMessage)
{
    ReceiveOnErrorOccurred(ErrorMessage);
}

void ARTSPlayerController::NotifyOnIssuedAttackOrder(APawn* OrderedPawn, AActor* Target)
{
	ReceiveOnIssuedAttackOrder(OrderedPawn, Target);
}

void ARTSPlayerController::NotifyOnIssuedMoveOrder(APawn* OrderedPawn, const FVector& TargetLocation)
{
    ReceiveOnIssuedMoveOrder(OrderedPawn, TargetLocation);
}

void ARTSPlayerController::NotifyOnIssuedStopOrder(APawn* OrderedPawn)
{
	ReceiveOnIssuedStopOrder(OrderedPawn);
}

void ARTSPlayerController::NotifyOnSelectionChanged(const TArray<AActor*>& Selection)
{
	RemovePendingOrder();
    ReceiveOnSelectionChanged(Selection);
}

void ARTSPlayerController::NotifyOnTeamChanged(ARTSTeamInfo* NewTeam)
{
	if (NewTeam)
	{
		// Notify listeners that new vision info is available now.
		// ARTSVisionInfo* VisionInfo = ARTSVisionInfo::GetVisionInfoForTeam(GetWorld(), NewTeam->TeamIndex);
		// NotifyOnVisionInfoAvailable(VisionInfo);
	}
}

// void ARTSPlayerController::NotifyOnVisionInfoAvailable(ARTSVisionInfo* VisionInfo)
// {
// 	// On server side, we're only interested in our own vision info.
// 	// Other player controllers that exist on the server for replication to the clients
// 	// should not affect rendering vision for the local player.
// 	if (this != GetWorld()->GetFirstPlayerController())
// 	{
// 		return;
// 	}
//
// 	// Allow others to setup vision.
// 	ReceiveOnVisionInfoAvailable(VisionInfo);
// }

void ARTSPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // Update sound cooldowns.
    if (SelectionSoundCooldownRemaining > 0.0f)
    {
        SelectionSoundCooldownRemaining -= DeltaTime;
    }

    APawn* PlayerPawn = GetPawn();

    if (!PlayerPawn)
    {
        return;
    }

	auto CameraComponent = PlayerPawn->FindComponentByClass<UCameraComponent>();
	
    // Get mouse input.
    float MouseX;
    float MouseY;

    const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

    const float ScrollBorderRight = ViewportSize.X - CameraScrollThreshold;
    const float ScrollBorderTop = ViewportSize.Y - CameraScrollThreshold;

    if (GetMousePosition(MouseX, MouseY))
    {
        if (MouseX <= CameraScrollThreshold)
        {
            CameraLeftRightAxisValue -= 1 - (MouseX / CameraScrollThreshold);
        }
        else if (MouseX >= ScrollBorderRight)
        {
            CameraLeftRightAxisValue += (MouseX - ScrollBorderRight) / CameraScrollThreshold;
        }

        if (MouseY <= CameraScrollThreshold)
        {
            CameraUpDownAxisValue += 1 - (MouseY / CameraScrollThreshold);
        }
        else if (MouseY >= ScrollBorderTop)
        {
            CameraUpDownAxisValue -= (MouseY - ScrollBorderTop) / CameraScrollThreshold;
        }
    }

    // Apply input.
    CameraLeftRightAxisValue = FMath::Clamp(CameraLeftRightAxisValue, -1.0f, +1.0f);
    CameraUpDownAxisValue = FMath::Clamp(CameraUpDownAxisValue, -1.0f, +1.0f);

    FVector Location = PlayerPawn->GetActorLocation();

    const FVector CameraForward = CameraComponent->GetForwardVector();
	
	FVector AdjustedForward(CameraForward.X, CameraForward.Y, 0.0f);
	AdjustedForward.Normalize();
	
	const FVector CameraRight = CameraComponent->GetRightVector();
	
    Location += CameraRight * CameraSpeed * CameraLeftRightAxisValue * DeltaTime;
    Location += AdjustedForward * CameraSpeed * CameraUpDownAxisValue * DeltaTime;

    PlayerPawn->SetActorLocation(Location);

    // Apply zoom input.
	USpringArmComponent* PlayerPawnSpringArm = PlayerPawn->FindComponentByClass<USpringArmComponent>();
	
	
    if (IsValid(PlayerPawnSpringArm))
    {
        float ZoomSpeed = CameraZoomSpeed * CameraZoomAxisValue * DeltaTime;
		PlayerPawnSpringArm->TargetArmLength += ZoomSpeed;// PlayerPawnSpringArm->TargetArmLength;
    }

	// Get hovered actors.
	HoveredActor = nullptr;

	TArray<FHitResult> HitResults;

	if (GetObjectsAtPointerPosition(HitResults))
	{
		for (auto& HitResult : HitResults)
		{
			// Check if hit any actor.
			if (HitResult.GetActor() == nullptr || Cast<ALandscape>(HitResult.GetActor()) != nullptr)
			{
				// Store hovered world position.
				HoveredWorldPosition = HitResult.Location;
			}
			
			// Check if hit selectable actor.
			auto SelectableComponent = HitResult.GetActor()->FindComponentByClass<URTSSelectableComponent>();

			if (!SelectableComponent)
			{
				HoveredWorldPosition = HitResult.Location;
				continue;
			}

			// Set hovered actor.
			HoveredActor = HitResult.GetActor();
		}
	}

	if (IsValid(PendingOrderPreviewActor))
	{
		bool bHasValidActor = false;
		for (auto Actor : SelectedActors)
		{
			PendingOrder.Target = HoveredActor;
			PendingOrder.bUseLocation = false;
			if (URTSOrderHelper::CheckOrder(Actor, PendingOrder).IsEmpty())
			{
				bHasValidActor = true;
				break;
			}

			PendingOrder.Location = FVector2D(HoveredWorldPosition);
			PendingOrder.bUseLocation = true;
			if (URTSOrderHelper::CheckOrder(Actor, PendingOrder).IsEmpty())
			{
				bHasValidActor = true;
				break;
			}
		}

		PendingOrderPreviewActor->SetActorHiddenInGame(!bHasValidActor);

		//UE_LOG(LogTemp, Warning, TEXT("Set Hidden %d"), !bHasValidActor);
		
		if (HoveredActor != nullptr)
		{
			PendingOrderPreviewActor->SetActorLocation(HoveredActor->GetActorLocation());
		}
		else
		{
			PendingOrderPreviewActor->SetActorLocation(HoveredWorldPosition);
		}

	}

	// Verify selection.
	SelectedActors.RemoveAll([=](AActor* SelectedActor) { return SelectedActor->IsHidden(); });
}
