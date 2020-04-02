// Fill out your copyright notice in the Description page of Project Settings.

#include "NonPlayerContainerWidget.h"
#include "MyGameInstance.h"
#include "InventorySystem/Utility/ArrayUtility.h"
#include "InventorySystemManager.h"
#include "InventorySystem/UI/InventorySlotWidget.h"
#include "InventorySystem/UI/ItemSlotBorder.h"
#include "InventorySystem/Components/ContainerAComp.h"


bool UNonPlayerContainerWidget::Open(UContainerAComp* container)
{
	if (GetVisibility() == ESlateVisibility::Hidden)
	{
		SetVisibility(ESlateVisibility::Visible);
		Settings.OwnerContainer = container;
		UWorld* world = GetWorld();
		if (world) {
			UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
			if (mgi) {

				SetParentContainer(container);
			}
		}
		return true;
	}
	return false;
}

bool UNonPlayerContainerWidget::Close()
{
	if (GetVisibility() == ESlateVisibility::Visible)
	{
		SetVisibility(ESlateVisibility::Hidden);
		Settings.OwnerContainer = nullptr;
		UWorld* world = GetWorld();
		if (world) {
			UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
			if (mgi) {

				mgi->InventorySystemManager->SetCurrentlyOpenNonPlayerContainer(nullptr);
			}
		}
		return true;
	}
	return false;
}

void UNonPlayerContainerWidget::SelectLeftItem() {
	UWorld* world = GetWorld();
	if (world) {

		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
		UInventorySystemManager* InvManager = mgi->InventorySystemManager;

		FIndex2D DesiredSelection = SelectedItem;
		FIndex2D PreviousSelection = FIndex2D();
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

void UNonPlayerContainerWidget::SelectRightItem() {
	UWorld* world = GetWorld();
	if (world) {

		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
		UInventorySystemManager* InvManager = mgi->InventorySystemManager;

		FIndex2D DesiredSelection = SelectedItem;
		FIndex2D PreviousSelection = FIndex2D();
		//DesiredSelection.x++;
		if (InvManager->InventorySystemState == EInventorySystemState::EInventoryAndLoot) {
			if (DesiredSelection.x == InvManager->GetGlobalContainerSlotsPerRow()-1) {
				DeselectItem(SelectedItem.x, SelectedItem.y);
				InvManager->SetCurrentlyBrowsedContainerWidget(*InvManager->GetPlayerContainerWidget());
				InvManager->GetCurrentlyBrowsedContainerWidget()->SelectItem(0, DesiredSelection.y);
				return;
			}
		}

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

void UNonPlayerContainerWidget::SelectUpperItem() {
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

void UNonPlayerContainerWidget::SelectLowerItem() {
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

void UNonPlayerContainerWidget::MoveSelectedItem() {

	UWorld* world = GetWorld();
	if (world) {

		if (UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>()) {

			if (UInventorySystemManager* InvManager = mgi->InventorySystemManager) {

				int32 OneDimCoord = From2DTo1D(SelectedItem.x, SelectedItem.y, InvManager->GetGlobalContainerSlotsPerRow());

				if (UItemSlotBorder* Slot = SlotArray2D[OneDimCoord]->GetItemSlotBorder()) {

					if (UItemAComp* Item = Slot->GetItemHeld()) {
						if (UContainerAComp* Container = InvManager->GetCurrentlyOpenNonPlayerContainer())
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