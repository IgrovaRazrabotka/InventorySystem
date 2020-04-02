// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerContainerWidget.h"
#include "UniformGridPanel.h"
#include "InventorySystem/Utility/ArrayUtility.h"
#include "WidgetTree.h"
#include "ItemSlotBorder.h"
#include "UniformGridSlot.h"
#include "MyGameInstance.h"
#include "InventorySystem/Components/ContainerAComp.h"
#include "InventorySystemManager.h"
#include "InventorySlotWidget.h"
#include "SelectionFrame.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

bool UPlayerContainerWidget::Open(UContainerAComp* container) {
	
	if (GetVisibility() == ESlateVisibility::Hidden)
	{
		SetVisibility(ESlateVisibility::Visible);
		Settings.OwnerContainer = container;
		UWorld* World = GetWorld();
		if (World) {
			UMyGameInstance* Gi = World->GetGameInstance<UMyGameInstance>();
			if (Gi) {
				
				SetParentContainer(container);
			}
		}
		return true;
	}
	return false;
}

bool UPlayerContainerWidget::Close() {
	
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SetVisibility(ESlateVisibility::Hidden);
		return true;
	}
	return false;
}

void UPlayerContainerWidget::SelectLeftItem() {
	UWorld* world = GetWorld();
	if (world) {

		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
		UInventorySystemManager* InvManager = mgi->InventorySystemManager;

		FIndex2D DesiredSelection = SelectedItem;
		FIndex2D PreviousSelection = FIndex2D();

		if(InvManager->InventorySystemState == EInventorySystemState::EInventoryAndLoot){
			if (DesiredSelection.x == 0) {
				if (UContainerWidget* Widget = InvManager->GetNonPlayerContainerWidget()) {
					DeselectItem(SelectedItem.x, SelectedItem.y);
					InvManager->SetCurrentlyBrowsedContainerWidget(*Widget);
					InvManager->GetCurrentlyBrowsedContainerWidget()->SelectItem(InvManager->GetGlobalContainerSlotsPerRow()-1, DesiredSelection.y);
					return;
				}
			}
		}

		int32 OneDimCoord = From2DTo1D(DesiredSelection.x, DesiredSelection.y, InvManager->GetGlobalContainerSlotsPerRow());
		OneDimCoord--;
		From1DTo2D(OneDimCoord, InvManager->GetGlobalContainerSlotsPerRow(), DesiredSelection.x, DesiredSelection.y);
		
		if (IsValidIndex2D(DesiredSelection.x, DesiredSelection.y,
			InvManager->GetGlobalContainerSlotsPerRow(),
			InvManager->GetGlobalContainerTotalSlotNumber())) {

			PreviousSelection = SelectedItem;
			SelectedItem = DesiredSelection;

			DeselectItem(PreviousSelection.x, PreviousSelection.y);

			SelectItem(SelectedItem.x, SelectedItem.y);
		}
	}
}

void UPlayerContainerWidget::SelectRightItem() {
	UWorld* world = GetWorld();
	if (world) {

		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
		UInventorySystemManager* InvManager = mgi->InventorySystemManager;

		FIndex2D DesiredSelection = SelectedItem;
		FIndex2D PreviousSelection = FIndex2D();
		int32 OneDimCoord = From2DTo1D(DesiredSelection.x, DesiredSelection.y, InvManager->GetGlobalContainerSlotsPerRow());
		OneDimCoord++;
		From1DTo2D(OneDimCoord, InvManager->GetGlobalContainerSlotsPerRow(), DesiredSelection.x, DesiredSelection.y);

		if (IsValidIndex2D(DesiredSelection.x, DesiredSelection.y,
			InvManager->GetGlobalContainerSlotsPerRow(),
			InvManager->GetGlobalContainerTotalSlotNumber())) {

			PreviousSelection = SelectedItem;
			SelectedItem = DesiredSelection;

			DeselectItem(PreviousSelection.x, PreviousSelection.y);

			SelectItem(SelectedItem.x, SelectedItem.y);
		}
	}
}

void UPlayerContainerWidget::SelectUpperItem() {
	UWorld* world = GetWorld();
	if (world) {

		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
		UInventorySystemManager* InvManager = mgi->InventorySystemManager;

		FIndex2D DesiredSelection = SelectedItem;
		FIndex2D PreviousSelection = FIndex2D();
		DesiredSelection.y--;

		if (IsValidIndex2D(DesiredSelection.x, DesiredSelection.y,
			InvManager->GetGlobalContainerSlotsPerRow(),
			InvManager->GetGlobalContainerTotalSlotNumber())) {

			PreviousSelection = SelectedItem;
			SelectedItem = DesiredSelection;

			DeselectItem(PreviousSelection.x, PreviousSelection.y);

			SelectItem(SelectedItem.x, SelectedItem.y);
		}
	}
}

void UPlayerContainerWidget::SelectLowerItem() {
	UWorld* world = GetWorld();
	if (world) {

		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
		UInventorySystemManager* InvManager = mgi->InventorySystemManager;

		FIndex2D DesiredSelection = SelectedItem;
		FIndex2D PreviousSelection = FIndex2D();
		DesiredSelection.y++;

		if (IsValidIndex2D(DesiredSelection.x, DesiredSelection.y,
			InvManager->GetGlobalContainerSlotsPerRow(),
			InvManager->GetGlobalContainerTotalSlotNumber())) {

			PreviousSelection = SelectedItem;
			SelectedItem = DesiredSelection;

			DeselectItem(PreviousSelection.x, PreviousSelection.y);

			SelectItem(SelectedItem.x, SelectedItem.y);
		}
	}
}

void UPlayerContainerWidget::MoveSelectedItem() {
	
	UWorld* world = GetWorld();
	if (world) {

		if (UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>()) {

			if (UInventorySystemManager* InvManager = mgi->InventorySystemManager) {

				int32 OneDimCoord = From2DTo1D(SelectedItem.x, SelectedItem.y, InvManager->GetGlobalContainerSlotsPerRow());

				if (UItemSlotBorder* Slot = SlotArray2D[OneDimCoord]->GetItemSlotBorder()) {

					if (UItemAComp* Item = Slot->GetItemHeld()) {
						if (UContainerAComp* Container = InvManager->GetPlayerContainer())
						{
							FIndex2D index;
							index.x = SelectedItem.x;
							index.y = SelectedItem.y;
							Container->MoveItem(*Item, index);
						}
					}
				}
			}
		}
	}
}


