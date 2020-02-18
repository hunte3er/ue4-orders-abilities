#include "RTSHUD.h"

#include "EngineUtils.h"
#include "Components/CapsuleComponent.h"

#include "RTSPlayerController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/RTSDefenseAttributeSet.h"


void ARTSHUD::DrawHUD()
{
	Super::DrawHUD();

	DrawFloatingCombatTexts();
	DrawSelectionFrame();
	DrawHealthBars();
	DrawHoveredActorEffect();
}

void ARTSHUD::NotifyDrawFloatingCombatText(AActor* Actor, const FString& Text, const FLinearColor& Color, float Scale, float Lifetime, float RemainingLifetime, float LifetimePercentage, float SuggestedTextLeft, float SuggestedTextTop)
{
	ReceiveDrawFloatingCombatText(
		Actor,
		Text,
		Color,
		Scale,
		Lifetime,
		RemainingLifetime,
		LifetimePercentage,
		SuggestedTextLeft,
		SuggestedTextTop);
}

void ARTSHUD::NotifyDrawHoveredActorEffect(AActor* HoveredActor)
{
	ReceiveDrawHoveredActorEffect(HoveredActor);
}

void ARTSHUD::NotifyDrawHealthBar(
	AActor* Actor,
	float CurrentHealth,
	float MaximumHealth,
	float HealthPercentage,
	float SuggestedHealthBarLeft,
	float SuggestedHealthBarTop,
	float SuggestedHealthBarWidth,
	float SuggestedHealthBarHeight)
{
	ReceiveDrawHealthBar(
		Actor,
		CurrentHealth,
		MaximumHealth,
		HealthPercentage,
		SuggestedHealthBarLeft,
		SuggestedHealthBarTop,
		SuggestedHealthBarWidth,
		SuggestedHealthBarHeight);
}

void ARTSHUD::NotifyDrawSelectionFrame(float ScreenX, float ScreenY, float Width, float Height)
{
	ReceiveDrawSelectionFrame(ScreenX, ScreenY, Width, Height);
}

FVector2D ARTSHUD::GetActorCenterOnScreen(AActor* Actor) const
{
	FVector ProjectedLocation = Project(Actor->GetActorLocation());
	return FVector2D(ProjectedLocation.X, ProjectedLocation.Y);
}

FVector2D ARTSHUD::GetActorSizeOnScreen(AActor* Actor) const
{
	// Get actor position projected on HUD.
	UCapsuleComponent* CapsuleComponent = Actor->FindComponentByClass<UCapsuleComponent>();

	if (!CapsuleComponent)
	{
		return FVector2D::ZeroVector;
	}

	FCollisionShape CollisionShape = CapsuleComponent->GetCollisionShape();

	FVector ActorTopPosition = Project(Actor->GetActorLocation() + (Actor->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
	FVector ActorBottomPosition = Project(Actor->GetActorLocation() - (Actor->GetActorForwardVector() * CollisionShape.Capsule.HalfHeight));
	FVector ActorLeftPosition = Project(Actor->GetActorLocation() - (Actor->GetActorRightVector() * CollisionShape.Capsule.Radius));
	FVector ActorRightPosition = Project(Actor->GetActorLocation() + (Actor->GetActorRightVector() * CollisionShape.Capsule.Radius));

	float Width = FVector(ActorRightPosition - ActorLeftPosition).Size();
	float Height = FVector(ActorTopPosition - ActorBottomPosition).Size();

	return FVector2D(Width, Height);
}

void ARTSHUD::DrawSelectionFrame()
{
	// Get selection frame.
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	FIntRect SelectionFrame;

	if (!PlayerController->GetSelectionFrame(SelectionFrame))
	{
		return;
	}

	// Draw selection frame.
	NotifyDrawSelectionFrame(
		SelectionFrame.Min.X,
		SelectionFrame.Min.Y,
		SelectionFrame.Width(),
		SelectionFrame.Height());
}

void ARTSHUD::DrawFloatingCombatTexts()
{
	if (!bShowFloatingCombatTexts)
	{
		return;
	}

	return;

	for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
	{
		AActor* Actor = *ActorIt;

		if (!IsValid(Actor))
		{
			return;
		}

		// // Check floating combat texts.
		// URTSFloatingCombatTextComponent* FloatingCombatTextComponent = Actor->FindComponentByClass<URTSFloatingCombatTextComponent>();
		//
		// if (!FloatingCombatTextComponent)
		// {
		// 	continue;
		// }
		//
		// for (FRTSFloatingCombatTextData& TextData : FloatingCombatTextComponent->Texts)
		// {
		// 	// Calculate lifetime.
		// 	float ElapsedLifetime = TextData.Lifetime - TextData.RemainingLifetime;
		// 	float LifetimePercentage = ElapsedLifetime / TextData.Lifetime;
		//
		// 	// Calculate position.
		// 	FVector2D Center = GetActorCenterOnScreen(Actor);
		// 	FVector2D Size = GetActorSizeOnScreen(Actor);
		//
		// 	// Calculate color.
		// 	FLinearColor TextColor = TextData.Color;
		//
		// 	if (bFadeOutFloatingCombatTexts)
		// 	{
		// 		TextColor.A = 1 - LifetimePercentage;
		// 	}
		//
		// 	// Draw text.
		// 	NotifyDrawFloatingCombatText(
		// 		Actor,
		// 		TextData.Text,
		// 		TextColor,
		// 		TextData.Scale,
		// 		TextData.Lifetime,
		// 		TextData.RemainingLifetime,
		// 		LifetimePercentage,
		// 		Center.X,
		// 		Center.Y - (Size.Y / 2) - (FloatingCombatTextSpeed * ElapsedLifetime));
		// }
	}
}

void ARTSHUD::DrawHealthBars()
{
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	// Check override conditions.
	if (bAlwaysShowHealthBars || (bShowHotkeyHealthBars && PlayerController->IsHealthBarHotkeyPressed()))
	{
		// Draw all health bars.
		for (TActorIterator<AActor> ActorIt(GetWorld()); ActorIt; ++ActorIt)
		{
			AActor* Actor = *ActorIt;
			DrawHealthBar(Actor);
		}

		return;
	}

	// Draw health bar for hovered actor.
	if (bShowHoverHealthBars)
	{
		AActor* HoveredActor = PlayerController->GetHoveredActor();
		DrawHealthBar(HoveredActor);
	}

	// Draw health bars for selected actors.
	if (bShowSelectionHealthBars)
	{
		for (int32 i = 0; i < PlayerController->GetSelectedActors().Num(); ++i)
		{
			AActor* SelectedActor = PlayerController->GetSelectedActors()[i];
			DrawHealthBar(SelectedActor);
		}
	}
}

void ARTSHUD::DrawHealthBar(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}

	// Check health.
	UAbilitySystemComponent* AbilitySystem = Actor->FindComponentByClass<UAbilitySystemComponent>();
	if (!AbilitySystem)
	{
		return;
	}

	const FGameplayAttribute HealthAttribute = URTSDefenseAttributeSet::GetHealthAttribute();
	const FGameplayAttribute MaxHealthAttribute = URTSDefenseAttributeSet::GetMaxHealthAttribute();
	if (!AbilitySystem->HasAttributeSetForAttribute(HealthAttribute) || !AbilitySystem->HasAttributeSetForAttribute(MaxHealthAttribute))
	{
		return;
	}
	const float Health = AbilitySystem->GetNumericAttribute(HealthAttribute);
	const float MaxHealth = AbilitySystem->GetNumericAttribute(MaxHealthAttribute);

	const float HealthPercentage = Health / MaxHealth;

	// Suggest health bar size.
	float SuggestedHealthBarLeft;
	float SuggestedHealthBarTop;
	float SuggestedHealthBarWidth;
	float SuggestedHealthBarHeight;

	SuggestUnitBarSize(Actor, SuggestedHealthBarLeft, SuggestedHealthBarTop, SuggestedHealthBarWidth, SuggestedHealthBarHeight);

	// Draw health bar.
	NotifyDrawHealthBar(
		Actor,
		Health,
		MaxHealth,
		HealthPercentage,
		SuggestedHealthBarLeft,
		SuggestedHealthBarTop,
		SuggestedHealthBarWidth,
		SuggestedHealthBarHeight);
}

void ARTSHUD::DrawHoveredActorEffect()
{
	ARTSPlayerController* PlayerController = Cast<ARTSPlayerController>(PlayerOwner);

	if (!PlayerController)
	{
		return;
	}

	AActor* HoveredActor = PlayerController->GetHoveredActor();

	if (HoveredActor == nullptr)
	{
		return;
	}

	// Draw hovered actor effect.
	NotifyDrawHoveredActorEffect(HoveredActor);
}

void ARTSHUD::SuggestUnitBarSize(AActor* Actor, float& OutLeft, float& OutTop, float& OutWidth, float& OutHeight) const
{
	FVector2D Center = GetActorCenterOnScreen(Actor);
	FVector2D Size = GetActorSizeOnScreen(Actor);

	OutWidth = Size.X * 2;
	OutHeight = 15.0f;

	OutLeft = Center.X - (OutWidth / 2);
	OutTop = Center.Y - (Size.Y / 2) - (OutHeight * 2);
}
