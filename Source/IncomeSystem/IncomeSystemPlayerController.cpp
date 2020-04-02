// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "IncomeSystemPlayerController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "IncomeSystemCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Engine.h"
#include "UserWidget.h"
#include "InventorySystem/UI/ContainerWidget.h"
#include "ConstructorHelpers.h"
#include "MyGameInstance.h"
#include "Engine/World.h"
#include "InventorySystem/Components/InteractableAComp.h"
#include "InventorySystemManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Framework/Application/SlateApplication.h"
#include "WidgetTree.h"

AIncomeSystemPlayerController::AIncomeSystemPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	bEnableMouseOverEvents = true;

}

void AIncomeSystemPlayerController::BeginPlay() {

	Super::BeginPlay();
	if (UWorld* World = GetWorld()) {
		if (UMyGameInstance* Gi = Cast<UMyGameInstance>(World->GetGameInstance())) {
			
			Gi->InventorySystemManager->GetPlayerContainerWidget()->bIsFocusable = true;

			//FSlateApplication& Slate = FSlateApplication::Get();
			//SPanel* Panel = Cast<SPanel>(Gi->InventorySystemManager->PlayerContainerWidget);
			//TSharedRef<SWidget> WidgetToFindPathTo = Focus->TakeWidget();
			//FWidgetPath FoundPath;
			//Slate.FindPathToWidget(WidgetToFindPathTo, FoundPath, EVisibility::All);
			//Focus->SetUserFocus(this);
			
			//Slate.SetUserFocus(uint32(0), FoundPath, EFocusCause::SetDirectly);
			
			//UWidget* Focus = Gi->InventorySystemManager->PlayerContainerWidget;// ->WidgetTree->FindWidget(TEXT("Focus"));
			//UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(this, Focus, EMouseLockMode::DoNotLock);
			//Focus->SetKeyboardFocus();

		}
	}

}

void AIncomeSystemPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		FHitResult Hit;
		TranceUnderCursor(Hit);
		MoveToMouseCursor(Hit);
	}
}

void AIncomeSystemPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();	

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AIncomeSystemPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AIncomeSystemPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("SetDestination", IE_Released, this, &AIncomeSystemPlayerController::OnWorldInteractReleased);
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AIncomeSystemPlayerController::OnToggleInventoryReleased);

	InputComponent->BindAction("HoldToDrop", IE_Pressed, this, &AIncomeSystemPlayerController::OnHoldToDropPressed);
	InputComponent->BindAction("HoldToDrop", IE_Released, this, &AIncomeSystemPlayerController::OnHoldToDropReleased);

	InputComponent->BindAction("MoveRight", IE_Pressed, this, &AIncomeSystemPlayerController::OnMoveRight);
	InputComponent->BindAction("MoveLeft", IE_Pressed, this, &AIncomeSystemPlayerController::OnMoveLeft);
	InputComponent->BindAction("MoveForth", IE_Pressed, this, &AIncomeSystemPlayerController::OnMoveForth);
	InputComponent->BindAction("MoveBack", IE_Pressed, this, &AIncomeSystemPlayerController::OnMoveBack);
	InputComponent->BindAction("Action", IE_Pressed, this, &AIncomeSystemPlayerController::OnAction);
	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AIncomeSystemPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AIncomeSystemPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AIncomeSystemPlayerController::OnResetVR);
}

void AIncomeSystemPlayerController::TranceUnderCursor(FHitResult& InOutHit) {
	// Trace to see what is under the mouse cursor
	GetHitResultUnderCursor(ECC_Visibility, false, InOutHit);
}

void AIncomeSystemPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AIncomeSystemPlayerController::MoveToMouseCursor(FHitResult& InOutHit)
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AIncomeSystemCharacter* MyPawn = Cast<AIncomeSystemCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
				//UNavigationSystem::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		if (InOutHit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(InOutHit.ImpactPoint);
		}
	}
}

void AIncomeSystemPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AIncomeSystemPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		UNavigationSystemBase* const NavSys = GetWorld()->GetNavigationSystem();
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if (NavSys && (Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
			//NavSys->SimpleMoveToLocation(this, DestLocation);
		}
	}
}

//World interaction

void AIncomeSystemPlayerController::WorldInteract(FHitResult& InOutHit) {
	AActor* hitActor = InOutHit.GetActor();

	if (hitActor) {
		if(UInteractableAComp* Interactable = Cast<UInteractableAComp>(hitActor->GetComponentByClass(UInteractableAComp::StaticClass())))
		{
			Interactable->Interact(GetCharacter());
		}
	}
}

void AIncomeSystemPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {
		if (!Gi->InventorySystemManager->GetPlayerContainerWidget()->IsOpened()) {
			bMoveToMouseCursor = true;
		}
	}
}

void AIncomeSystemPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {
		if (!Gi->InventorySystemManager->GetPlayerContainerWidget()->IsOpened()) {
			bMoveToMouseCursor = false;
		}
	}
}

void AIncomeSystemPlayerController::OnWorldInteractReleased()
{
	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {
		if (!Gi->InventorySystemManager->GetPlayerContainerWidget()->IsOpened()) {
			FHitResult Hit;
			TranceUnderCursor(Hit);
			WorldInteract(Hit);
		}
	}
}

void AIncomeSystemPlayerController::OnToggleInventoryReleased() {
	
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance) {
		UContainerWidget* PlayerWidget = GameInstance->InventorySystemManager->GetPlayerContainerWidget();
		UContainerWidget* NonPlayerWidget = GameInstance->InventorySystemManager->GetNonPlayerContainerWidget();
		if (PlayerWidget) {
			UContainerAComp* PlayerContainer = Cast<UContainerAComp>(GetPawn()->GetComponentByClass(UContainerAComp::StaticClass()));
			if(GameInstance->InventorySystemManager->InventorySystemState == EInventorySystemState::EClosed){
				GameInstance->InventorySystemManager->EnterInventoryOnlyState(PlayerWidget, PlayerContainer);
			}
			else if (GameInstance->InventorySystemManager->InventorySystemState == EInventorySystemState::EInventoryOnly) {
				GameInstance->InventorySystemManager->ExitInventoryOnlyState(PlayerWidget);
			}
			else if (GameInstance->InventorySystemManager->InventorySystemState == EInventorySystemState::EInventoryAndLoot) {
				GameInstance->InventorySystemManager->ExitInventoryAndLootState(PlayerWidget, NonPlayerWidget);
			}
		}
	}
}

void AIncomeSystemPlayerController::OnHoldToDropPressed()
{
	UMyGameInstance* mgi = Cast<UMyGameInstance>(GetGameInstance());
	if (mgi) {
		mgi->InventorySystemManager->SetHoldToDrop(true);
	}
}

void AIncomeSystemPlayerController::OnHoldToDropReleased()
{
	UMyGameInstance* mgi = Cast<UMyGameInstance>(GetGameInstance());
	if (mgi) {
		mgi->InventorySystemManager->SetHoldToDrop(false);
	}
}

void AIncomeSystemPlayerController::OnMoveRight() {

	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {
		if (Gi->InventorySystemManager->InventorySystemState != EInventorySystemState::EClosed) {
			Gi->InventorySystemManager->GetCurrentlyBrowsedContainerWidget()->SelectRightItem();
		}
	}
}

void AIncomeSystemPlayerController::OnMoveLeft() {

	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {
		if (Gi->InventorySystemManager->InventorySystemState != EInventorySystemState::EClosed) {
			Gi->InventorySystemManager->GetCurrentlyBrowsedContainerWidget()->SelectLeftItem();
		}
	}
}

void AIncomeSystemPlayerController::OnMoveForth() {

	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {
		if (Gi->InventorySystemManager->InventorySystemState != EInventorySystemState::EClosed) {
			Gi->InventorySystemManager->GetCurrentlyBrowsedContainerWidget()->SelectUpperItem();
		}
	}
}

void AIncomeSystemPlayerController::OnMoveBack() {

	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {
		if (Gi->InventorySystemManager->InventorySystemState != EInventorySystemState::EClosed) {
			Gi->InventorySystemManager->GetCurrentlyBrowsedContainerWidget()->SelectLowerItem();
		}
	}
}

void AIncomeSystemPlayerController::OnAction() {

	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {
		if (Gi->InventorySystemManager->InventorySystemState == EInventorySystemState::EInventoryOnly) {
			if (Gi->InventorySystemManager->GetHoldToDrop()) {
				Gi->InventorySystemManager->GetPlayerContainerWidget()->DropSelectedItem();
			}
			else {
				Gi->InventorySystemManager->GetPlayerContainerWidget()->UseSelectedItem();
			}
		}
		else if (Gi->InventorySystemManager->InventorySystemState == EInventorySystemState::EInventoryAndLoot) {
			if (Gi->InventorySystemManager->GetCurrentlyBrowsedContainerWidget()) {
				Gi->InventorySystemManager->GetCurrentlyBrowsedContainerWidget()->MoveSelectedItem();
			}
		}
	}
}
