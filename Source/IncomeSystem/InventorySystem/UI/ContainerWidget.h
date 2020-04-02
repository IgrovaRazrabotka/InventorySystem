// Fill out your copyright notice in the Description page of Project Settings.

//This is the container widget/window class. It is responsible for initializing and handling opening/closing of windows/widgets.
//This class is a base for the PlayerContainerWidget.h and the NonPlayerContainerWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySystem/Utility/ItemArray3D.h"
#include "ContainerWidget.generated.h"

/**
 * 
 */

class UContainerAComp;
class UInventorySlotWidget;
class UItemSlotBorder;

USTRUCT(BlueprintType)
struct FContainerWidgetSettings
{
	GENERATED_BODY()

		FContainerWidgetSettings() {};

	UPROPERTY()
		UContainerAComp* OwnerContainer;
};

UCLASS(Abstract)
class INCOMESYSTEM_API UContainerWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UContainerWidget(const FObjectInitializer& ObjectInitializer);
	
	//We override the existing Initialize method to do our own initialization of the widget/window
	virtual bool Initialize();

	//Open the widget/window and do the needed set-up things at the same time. Although this method takes a parameter, this parameter is NOT needed
	//for the player override as the player is a special case. (Just pass "nullptr" in the player case - no parameter needed in that case).
	//The player setup is done in another place at the start of the game - see the ::PreInitializeComponents() method in the InventorySystemCharacter.h/.cpp files.
	virtual bool Open(UContainerAComp* container);

	//This is the counterpart of Open. It closes a window that has been opened and performs the needed settings.
	virtual bool Close();

	//This method checks if the widget/window is closed and returns true if it is.
	virtual bool IsClosed();

	//This method checks if the widget/window is opened and returns true if it is.
	virtual bool IsOpened();
	
	//This method toggles between Open and Close. Id it IsClosed it will Open; if it IsOpened it will Close.
	virtual bool ToggleOpenClose(UContainerAComp* container);

	//This method gets you the owning container of this widget. The owner container is set to NULL when non-player containers/windows/widgets are closed.
	virtual UContainerAComp* GetOwnerContainer();

	//This method updates the slots of the widget/window with the content provided through the parameter.
	//You would not need to call this ever as the system updates the widget/window as necessary.
	virtual void Update(UItemArray3D* items);

protected:
	virtual void WipeAllSlots(UItemArray3D* items);
	virtual void ProcessOffsetGreaterThanZero(int32 x, int32 y, int32& offset, UItemSlotBorder* slot, UItemArray3D* items, UInventorySlotWidget* widget);
	virtual void ProcessOffsetZero(int32 x, int32 y, UItemSlotBorder* slot, UItemArray3D* items, UInventorySlotWidget* widget);
public:
	//Sets the parent of all slots and to the widget/window itself to the parameter that is provided and updates the widget.
	//This is needed in exotic cases only and you would not neet to use it normally.
	virtual bool SetParentContainer(UContainerAComp* container);

	virtual void UseSelectedItem();

	virtual void DropSelectedItem();

	virtual void MoveSelectedItem();

	virtual void SelectLeftItem();
	virtual void SelectRightItem();
	virtual void SelectUpperItem();
	virtual void SelectLowerItem();
	virtual void SelectItem(int32 xPos, int32 yPos);
	virtual void DeselectItem(int32 xPos, int32 yPos);
	virtual void SelectLastSelectedItem();
	virtual void DeselectCurrentlySelectedItem();
protected:

	//This field holds the settings of this widget/window
	UPROPERTY()
	FContainerWidgetSettings Settings;
	
	//This field holds actual array of slots for thw widget/window
	UPROPERTY()
	TArray<UInventorySlotWidget*> SlotArray2D;

	FIndex2D SelectedItem;
	
};
