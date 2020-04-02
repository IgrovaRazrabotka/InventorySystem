// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

//In this file we add some more controls to the existing ones and we create their implementation.
//These implementations make sense for a Top Down game but you can override the functionality to match your
//specific needs.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "IncomeSystemPlayerController.generated.h"

UCLASS()
class AIncomeSystemPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AIncomeSystemPlayerController();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;
	

	// Begin PlayerController interface
	virtual void PlayerTick(float DeltaTime) override;

	//This method adds the "Drop" item and "ToggleInventory" key pressing functionalities.
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	virtual void SetupInputComponent();
	//virtual void SetupInputComponent_Implementation();
	// End PlayerController interface


	// tracing
	void TranceUnderCursor(FHitResult& InOutHit);

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Navigate player to the current mouse cursor location. */
	void MoveToMouseCursor(FHitResult& InOutHit);

	/** Navigate player to the current touch location. */
	void MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location);
	
	/** Navigate player to the given world location. */
	void SetNewMoveDestination(const FVector DestLocation);

	// Interaction with the world
	// This method handles the case when the player clicks the mouse button (clicks on an item in the world).
	virtual void WorldInteract(FHitResult& InOutHit);

	/** Input handlers for SetDestination action. */
	void OnSetDestinationPressed();
	void OnSetDestinationReleased();

	//World Interaction
	//This method simply handles the case when the mouse button is released (or when a mouse click has been performed).
	UFUNCTION()
	virtual void OnWorldInteractReleased();
	//virtual void OnWorldInteractReleased_Implementation();
	
	//Inventory
	//This method handles the case when the inventory toggle button has been released (or when the inventory button has been pressed).
	UFUNCTION()
	virtual void OnToggleInventoryReleased();
	//virtual void OnToggleInventoryReleased_Implementation();

	//Inventory
	//This method changes a flag / bool when the "Drop" key is pressed.You should not call this method directly in the general case.
	UFUNCTION()
	virtual void OnHoldToDropPressed();
	//virtual void OnHoldToDropPressed_Implementation();

	//This method changes a flag / bool when the "Drop" key is released.You should not call this method directly in the general case.
	UFUNCTION()
	virtual void OnHoldToDropReleased();
	//virtual void OnHoldToDropReleased_Implementation();

	UFUNCTION()
	virtual void OnMoveRight();

	UFUNCTION()
		virtual void OnMoveLeft();

	UFUNCTION()
		virtual void OnMoveForth();

	UFUNCTION()
		virtual void OnMoveBack();
	
	UFUNCTION()
		virtual void OnAction();

};


