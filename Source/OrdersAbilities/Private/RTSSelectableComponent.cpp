
#include "RTSSelectableComponent.h"

#include "OrdersAbilities/OrdersAbilities.h"
#include "RTSPlayerController.h"

#include "WorldCollision.h"
#include "Components/CapsuleComponent.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/MeshComponent.h"
#include "Orders/RTSOrderHelper.h"
#include "AbilitySystem/RTSAbilitySystemHelper.h"


void URTSSelectableComponent::BeginPlay()
{
	Super::BeginPlay();
	
    AActor* Owner = GetOwner();

    if (!IsValid(Owner))
    {
        return;
    }

    UCapsuleComponent* CapsuleComponent = Owner->FindComponentByClass<UCapsuleComponent>();

    if (IsValid(CapsuleComponent))
    {
        // Create selection circle decal.
        DecalComponent = NewObject<UDecalComponent>(Owner, TEXT("SelectionCircleDecal"));

        if (DecalComponent)
        {
            DecalComponent->RegisterComponent();
            DecalComponent->AttachToComponent(Owner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

            // Set decal size.
            FCollisionShape CollisionShape = CapsuleComponent->GetCollisionShape();
            float DecalHeight = CollisionShape.Capsule.HalfHeight * 2;
            float DecalRadius = CollisionShape.Capsule.Radius;

            DecalComponent->DecalSize = FVector(DecalHeight, DecalRadius, DecalRadius);

            // Rotate decal to face ground.
            DecalComponent->SetRelativeRotation(FRotator::MakeFromEuler(FVector(0.0f, -90.0f, 0.0f)));

            // Setup decal material.
            SelectionCircleMaterialInstance = UMaterialInstanceDynamic::Create(SelectionCircleMaterial, this);
            DecalComponent->SetDecalMaterial(SelectionCircleMaterialInstance);

            DecalComponent->SetHiddenInGame(true);
        }
    }
    else
    {
        UE_LOG(LogRTS, Warning, TEXT("%s has no UCapsuleComponent and won't show selection circles."), *Owner->GetName());
    }

	TArray<UMeshComponent*> Meshes;
	GetOwner()->GetComponents<UMeshComponent>(Meshes);
	for (UMeshComponent* MeshComponent : Meshes)
	{
		MeshComponent->SetRenderCustomDepth(true);
		MeshComponent->SetCustomDepthStencilValue(255);
	}
}

void URTSSelectableComponent::SelectActor(ARTSPlayerController* SelectedBy)
{
	if (bSelected)
	{
		return;
	}

	bSelected = true;

 //    AActor* OwnerActor = GetOwner();
 //    ARTSPlayerState* OwnerPlayerState = nullptr;
	// if (auto Pawn = Cast<APawn>(OwnerActor))
	// {
 //        OwnerPlayerState = Cast<ARTSPlayerState>(Pawn->GetPlayerState());
	// }

    // FGameplayTagContainer RelationshipTags;
    // URTSAbilitySystemHelper::GetRelationshipTagsFromPlayers(SelectedBy->GetPlayerState(), OwnerPlayerState, RelationshipTags);
	
    if (SelectedBy->DoesControllerOwnActor(GetOwner()))
    {
        SelectionCircleMaterialInstance->SetVectorParameterValue("Color", FriendlyColor);
    }
    else
    {
        SelectionCircleMaterialInstance->SetVectorParameterValue("Color", NeutralColor);
    }

    // Update selection circle.
    if (IsValid(DecalComponent))
    {
        DecalComponent->SetHiddenInGame(false);
    }

	// Notify listeners.
	OnSelected.Broadcast();
}

void URTSSelectableComponent::DeselectActor()
{
	if (!bSelected)
	{
		return;
	}

	bSelected = false;

    // Update selection circles.
    if (IsValid(DecalComponent))
    {
        DecalComponent->SetHiddenInGame(true);
    }

	// Notify listeners.
	OnDeselected.Broadcast();
}

bool URTSSelectableComponent::IsSelected()
{
	return bSelected;
}
