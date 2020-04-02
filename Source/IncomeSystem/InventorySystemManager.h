// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventorySystem/UI/GameplayGroupWidget.h"
#include "InventorySystem/UI/PlayerContainerWidget.h"
#include "InventorySystemManager.generated.h"


class UPlayerContainerWidget;
class UContainerWidget;
class UUserWidget;
class UItemFactory;
class UTooltipWindowWidget;
class UItemSlotBorder;

enum class EInventorySystemState {
	EInventoryOnly,
	EInventoryAndLoot,
	EClosed
};

UCLASS()
class INCOMESYSTEM_API UInventorySystemManager : public UObject
{
	GENERATED_BODY()
	friend class UMyGameInstance;
public:

	UInventorySystemManager();

	EInventorySystemState InventorySystemState = EInventorySystemState::EClosed;

	const float InteractionDistance = 200.0f;

private:
	//This is the flag that corresponds to the pressing of the "Drop" key binding
	UPROPERTY()
	bool bHoldToDrop = false;

	//This is the main widget that contains all the other UMG widgets. It is a technical requirement all widgets to be inside one parent widget. (focus problem)
	//Do not change the name or the the path of the resource that is loaded into this class. See GameplayGroupWidget.h for more info.
	UPROPERTY()
	TSubclassOf<UUserWidget> GameplayGroupWidgetClass;

	//This is the actual widget that is created from the GameplayGroupWidgetClass
	UPROPERTY()
	UGameplayGroupWidget* GameplayGroupWidget;

	UPROPERTY()
	UPlayerContainerWidget* PlayerContainerWidget;

	UPROPERTY()
	UContainerWidget* CurrentlyBrowsedContainerWidget;

	//This is the slot button complex that is percieved as an "item" in the container window/widget. It is loaded from a resource. The resource may be edited but
	//should not change its name or path
	UPROPERTY()
	TSubclassOf<UUserWidget> SlotButtonWidgetClass;

	//These are variables for the player and the currently open non-player containers
	UPROPERTY()
	UContainerAComp* PlayerContainer;
	//This container may be null when no container of the non-player kind is opened
	UPROPERTY()
	UContainerAComp* CurrentlyOpenNonPlayerContainer;

	//Item factory
//This is a utility for easy spawning. See ItemFactory.h for more info.
	UPROPERTY()
	UItemFactory* ItemFactory;

	UPROPERTY()
	UItemSlotBorder * LastHoveredSlotButton = nullptr;

public:

	void EnterInventoryOnlyState(UContainerWidget* PlayerWidget, UContainerAComp* PlayerContainer);
	void EnterInventoryAndLootState(UContainerAComp* _NonPlayerContainer, UContainerWidget* _CurrentlyBrowsedContainerWidget);

	void ExitInventoryOnlyState(UContainerWidget* PlayerWidget);
	void ExitInventoryAndLootState(UContainerWidget* PlayerWidget, UContainerWidget* NonPlayerWidget);

	UContainerWidget* GetCurrentlyBrowsedContainerWidget();
	void SetCurrentlyBrowsedContainerWidget(UContainerWidget& ContainerWidget);

	//Get the player container widget (UI/UMG visual)
	UContainerWidget * GetPlayerContainerWidget();
	void SetPlayerContainerWidget(UPlayerContainerWidget& PlayerContainerWidget);
	
	//Get the non-player container widget (UI/UMG visual)
	UContainerWidget * GetNonPlayerContainerWidget();

	UTooltipWindowWidget * GetTooltipWindowWidget();

	//A getter for the SlotButtonWidgetClass
	TSubclassOf<UUserWidget> GetContainerSlotClass();

	//Get the player container. It is assigned early on game startup and does not change.
	UContainerAComp* GetPlayerContainer();

	bool SetPlayerContainer(UContainerAComp* container);

	//Get the currently opened non-player container. It is changed every time a container is opened. It may return null of no container is currently opened.
	UContainerAComp* GetCurrentlyOpenNonPlayerContainer();
	//Set the currently opened non-player container. Should be changed every time a new non-player contaienr is Opened. It may be null of no container is currently opened.
	
	bool SetCurrentlyOpenNonPlayerContainer(UContainerAComp* container);

//Get the global horizontal dimension or width size (the x axis) of the inventory container window. You can change the value inside to change global inventories size.

	int32 GetGlobalContainerSlotsPerRow();
	//Get the global vertical dimension or height size (the y axis) of the inventory container window. You can change the value inside to change global inventories size.
	
	int32 GetGlobalContainerSlotsPerColumn();
	//Get the total number of slots that any container has.
	
	int32 GetGlobalContainerTotalSlotNumber();

	//Input
//This function sets the flag if the Drop keyboard key is currently held own. You should not call this manually as it is called by the keypress event.
//Only call manually if you know what you are doing
	void SetHoldToDrop(bool flag);
	//Get the flag of the Drop keyboard key. If it is true - we are trying to drop an item
	
	bool GetHoldToDrop();

	//Item factory
//Get the item factory instance. The item factory allows for easy spawning of game objects. See ItemFactory.h for more details.
	UItemFactory* GetItemFactory();
};
