// Fill out your copyright notice in the Description page of Project Settings.

#include "ContainerWidget.h"
#include "MyGameInstance.h"
#include "Engine/Texture2D.h"
#include "Button.h"
#include "ItemSlotBorder.h"
#include "WidgetTree.h"
#include "UniformGridPanel.h"
#include "UniformGridSlot.h"
#include "InventorySystem/Utility/ArrayUtility.h"
#include "Engine.h"
#include "MyGameInstance.h"
#include "ConstructorHelpers.h"
#include "InventorySlotWidget.h"
#include "MyGameInstance.h"
#include "TextBlock.h"
#include "InventorySystem/Utility/ArrayUtility.h"
#include "InventorySystem/Components/ContainerAComp.h"
#include "InventorySystemManager.h"
#include "SelectionFrame.h"


UContainerWidget::UContainerWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	
}

bool UContainerWidget::Initialize() {
	
	Super::Initialize();

	bIsEnabled = true;

	UWorld* world = GetWorld();
	if (world) {
		
		if (UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>()) {

			if (UInventorySystemManager* InvManager = mgi->InventorySystemManager) {
				//The default selected slot is
				SelectedItem.x = 0;
				SelectedItem.y = 0;

				SlotArray2D.Init(nullptr, InvManager->GetGlobalContainerTotalSlotNumber());
				UUniformGridPanel* SlotsGrid = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("SlotsGrid")));

				if (GetWorld() && SlotsGrid) {

					for (int32 col = 0; col < InvManager->GetGlobalContainerSlotsPerColumn(); col++) {

						for (int32 row = 0; row < InvManager->GetGlobalContainerSlotsPerRow(); row++) {

							if (IsValidIndex2D(row, col, InvManager->GetGlobalContainerSlotsPerRow(), InvManager->GetGlobalContainerTotalSlotNumber())) {

								FString name = FString::Printf(TEXT("Slot_R%d_C%d"), row, col);

								UClass* containerSlotClass = Cast<UClass>(InvManager->GetContainerSlotClass());
								UInventorySlotWidget* NewInventorySlotWidget = WidgetTree->ConstructWidget<UInventorySlotWidget>(containerSlotClass, FName(*name));
								UItemSlotBorder* NewSlot = NewInventorySlotWidget->GetItemSlotBorder();
								UTextBlock* NewStackNumber = NewInventorySlotWidget->GetStackNumber();
								USelectionFrame* NewSelectionFrame = NewInventorySlotWidget->GetSelectionFrame();

								if (NewSlot) {

									FItemSlotBorderSettings NewSettings;
									//blueprints do not support uint32, but only int32
									NewSettings.index.x = row;
									NewSettings.index.y = col;
									NewSettings.ParentContainer = nullptr;
									NewSettings.ItemHeld = nullptr;
									// Initialize the button so that it knows its coordinates if its reference
									// in the item container. We do this as Unreal c++ does not support constructors with arguments
									NewSlot->Init(NewSettings);
								}

								if (NewStackNumber) {
									FString newText = "";
									NewStackNumber->SetText(FText::FromString(newText));
								}

								if (NewSelectionFrame) {
									NewSelectionFrame->SetState(false);
								}

								UUniformGridSlot* Slot = SlotsGrid->AddChildToUniformGrid(NewInventorySlotWidget);

								//Important: we swap the row and col intentionally here - unreal flips x and y (horizontal and vertical).
								Slot->SetRow(col);
								Slot->SetColumn(row);
								Slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
								Slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

								SetElement2D(row, col, InvManager->GetGlobalContainerSlotsPerRow(), SlotArray2D, NewInventorySlotWidget);
							}
						}
					}
				}
			}
		}
	}
	return true;
}

bool UContainerWidget::Open(UContainerAComp* container)
{
	return true;
}

bool UContainerWidget::Close()
{
	return true;
}

bool UContainerWidget::IsClosed() {
	return GetVisibility() == ESlateVisibility::Hidden;
}

bool UContainerWidget::IsOpened() {
	return GetVisibility() == ESlateVisibility::Visible;
}

bool UContainerWidget::ToggleOpenClose(UContainerAComp* container) {
	if (IsOpened()) {
		Close();
		return true;
	}
	else if(IsClosed()) {
		Open(container);
		return true;
	}
	UE_LOG(UnhandledCase, Warning, TEXT("Unhandled case in UContainerWidget::ToggleOpenClose()"));
	return false;
}

UContainerAComp* UContainerWidget::GetOwnerContainer()
{
	return Settings.OwnerContainer;
}

void UContainerWidget::Update(UItemArray3D* items) {
	
	UWorld* world = GetWorld();
	if (world) {
		if(UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>()) {
			if (items) {

				UInventorySystemManager* InvManager = mgi->InventorySystemManager;
				if(UUniformGridPanel* SlotsGrid = Cast<UUniformGridPanel>(GetWidgetFromName(TEXT("SlotsGrid")))) {
					//Wipe all
					WipeAllSlots(items);
					//Set all.
					//Offset will always be 1 at most and 0 at least.
					int32 offset = 0;
					int32 originCorrection = 0;
					//It is important to traverse the container in a row first fassion - complete the row from left to right,
					//then go to next row (top to bottom).
					for (int32 y = 0; y < InvManager->GetGlobalContainerSlotsPerColumn(); y++) {

						for (int32 x = 0; x < InvManager->GetGlobalContainerSlotsPerRow(); x++) {

							if(UInventorySlotWidget* widget = GetPtrToElement2D(x, y, InvManager->GetGlobalContainerSlotsPerRow(), SlotArray2D)) {
								
								if(UItemSlotBorder* slot = widget->GetItemSlotBorder()) {
									if (items->dimension[x]->dimension[y]->dimension.Num() > 0) 
									{										
										if (offset > 0) ProcessOffsetGreaterThanZero(x,y,offset,slot,items,widget);
										else ProcessOffsetZero(x, y, slot, items, widget);
										
										originCorrection++;
									}
									else offset++;
								}
							}
						}
					}
				}
			}
		}
	}
}

void UContainerWidget::WipeAllSlots(UItemArray3D* items) {
	
	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {

		for (int32 x = 0; x < items->dimension.Num(); x++) {

			for (int32 y = 0; y < items->dimension[x]->dimension.Num(); y++) {

				UInventorySlotWidget* widget = GetPtrToElement2D(x, y, Gi->InventorySystemManager->GetGlobalContainerSlotsPerRow(), SlotArray2D);
				if (widget) {
					UItemSlotBorder* slot = widget->GetItemSlotBorder();
					if (slot) {
						slot->SetContents(nullptr, FIndex2D());

						UTextBlock* text = widget->GetStackNumber();
						if (text) {
							FString newText = "";
							text->SetText(FText::FromString(newText));
						}
					}
				}
			}
		}
	}
}

void UContainerWidget::ProcessOffsetGreaterThanZero(int32 x, int32 y, int32& offset, UItemSlotBorder* slot, UItemArray3D* items, UInventorySlotWidget* widget) {
	
	if (UMyGameInstance* Gi = Cast<UMyGameInstance>(GetGameInstance())) {
		FIndex2D OutIndex;
		//UInventorySlotWidget* newWidget = nullptr;
		//Convert the coordinate to 1D; subtract the offset; get the coordinate back to 2D.
		int32 tmp1D = From2DTo1D(x, y, Gi->InventorySystemManager->GetGlobalContainerSlotsPerRow());
		tmp1D -= offset;
		From1DTo2D(tmp1D, Gi->InventorySystemManager->GetGlobalContainerSlotsPerRow(), OutIndex.x, OutIndex.y);

		if (UInventorySlotWidget* newWidget = GetPtrToElement2D(OutIndex.x, OutIndex.y, Gi->InventorySystemManager->GetGlobalContainerSlotsPerRow(), SlotArray2D)) {
			//Set the old button to contain no items and indices
			slot->SetContents(nullptr, FIndex2D());
				//Set the text of the old widget to be NONE
				if (UTextBlock* text = widget->GetStackNumber()) {
					FString theText = "";
						text->SetText(FText::FromString(theText));
				}
			//Set the NEW button of the NEW widget slot to contain an item and indices.
			UItemSlotBorder* newSlot = newWidget->GetItemSlotBorder();
				newSlot->SetContents(*items->dimension[x]->dimension[y]->dimension.GetData(), OutIndex);
				//Set the NEW text of the NEW widget to contain the right stack info.
				if (UTextBlock* newText = newWidget->GetStackNumber()) {
					if (items->dimension[x]->dimension[y]->dimension.Num() > 1) {
						FString theText = "x" + FString::FromInt(items->dimension[x]->dimension[y]->dimension.Num());
						newText->SetText(FText::FromString(theText));
					}
					else {
						FString theText = "";
						newText->SetText(FText::FromString(theText));
					}
				}
			//Important: Adjust the item in the actual container to fix indexing error when removing or using item
			items->dimension[OutIndex.x]->dimension[OutIndex.y]->dimension = items->dimension[x]->dimension[y]->dimension;
			items->dimension[x]->dimension[y]->dimension.Empty();
		}
	}
}

void UContainerWidget::ProcessOffsetZero(int32 x, int32 y, UItemSlotBorder* slot, UItemArray3D* items, UInventorySlotWidget* widget) {
	
	FIndex2D ind;
	ind.x = x;
	ind.y = y;

	slot->SetContents(*items->dimension[x]->dimension[y]->dimension.GetData(), ind);

	UTextBlock* text = widget->GetStackNumber();
	if (text) {
		if (items->dimension[x]->dimension[y]->dimension.Num() > 1) {
			FString newText = "x" + FString::FromInt(items->dimension[x]->dimension[y]->dimension.Num());
			text->SetText(FText::FromString(newText));
		}
		else {
			FString newText = "";
			text->SetText(FText::FromString(newText));
		}
	}
}

bool UContainerWidget::SetParentContainer(UContainerAComp* container) {
	
	Settings.OwnerContainer = container;
	if (container != nullptr) {
			Update(container->GetItems());
	}

	for (UInventorySlotWidget* widget : SlotArray2D) {
		if (widget) {
			if (UItemSlotBorder* slot = widget->GetItemSlotBorder())
			{
				if (slot) {
					slot->SetParentContainer(container);
				}
			}
		}
	}
	
	return true;
}

void UContainerWidget::SelectItem(int32 xPos, int32 yPos) {

	UWorld* world = GetWorld();
	if (world) {

		if (UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>()) {

			if (UInventorySystemManager* InvManager = mgi->InventorySystemManager) {

				int32 OneDimCoord = From2DTo1D(xPos, yPos, InvManager->GetGlobalContainerSlotsPerRow());
					
				if (USelectionFrame* Frame = SlotArray2D[OneDimCoord]->GetSelectionFrame()) {

					if (Frame) {
						SelectedItem.x = xPos;
						SelectedItem.y = yPos;
						Frame->SetState(true);
					}
				}
			}
		}
	}
}

void UContainerWidget::DeselectItem(int32 xPos, int32 yPos) {

	UWorld* world = GetWorld();
	if (world) {

		if (UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>()) {

			if (UInventorySystemManager* InvManager = mgi->InventorySystemManager) {

				int32 OneDimCoord = From2DTo1D(xPos, yPos, InvManager->GetGlobalContainerSlotsPerRow());

				if (USelectionFrame* Frame = SlotArray2D[OneDimCoord]->GetSelectionFrame()) {

					if (Frame) {
						Frame->SetState(false);
					}
				}
			}
		}
	}
}

void UContainerWidget::SelectLastSelectedItem() {

	UWorld* world = GetWorld();
	if (world) {

		if (UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>()) {

			if (UInventorySystemManager* InvManager = mgi->InventorySystemManager) {

				int32 OneDimCoord = From2DTo1D(SelectedItem.x, SelectedItem.y, InvManager->GetGlobalContainerSlotsPerRow());

				if (USelectionFrame* Frame = SlotArray2D[OneDimCoord]->GetSelectionFrame()) {

					if (Frame) {
						Frame->SetState(true);
					}
				}
			}
		}
	}
}

void UContainerWidget::DeselectCurrentlySelectedItem() {

	UWorld* world = GetWorld();
	if (world) {

		if (UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>()) {

			if (UInventorySystemManager* InvManager = mgi->InventorySystemManager) {

				int32 OneDimCoord = From2DTo1D(SelectedItem.x, SelectedItem.y, InvManager->GetGlobalContainerSlotsPerRow());

				if (USelectionFrame* Frame = SlotArray2D[OneDimCoord]->GetSelectionFrame()) {

					if (Frame) {
						Frame->SetState(false);
					}
				}
			}
		}
	}
}

void UContainerWidget::UseSelectedItem() {

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
							Container->UseItem(*Item, index);
						}
					}
				}
			}
		}
	}
}

void UContainerWidget::DropSelectedItem() {

	UWorld* world = GetWorld();
	if (world) {

		if (UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>()) {

			if (UInventorySystemManager* InvManager = mgi->InventorySystemManager) {

				int32 OneDimCoord = From2DTo1D(SelectedItem.x, SelectedItem.y, mgi->InventorySystemManager->GetGlobalContainerSlotsPerRow());

				if (UItemSlotBorder* Slot = SlotArray2D[OneDimCoord]->GetItemSlotBorder()) {

					if (UItemAComp* Item = Slot->GetItemHeld()) {
						if (UContainerAComp* Container = InvManager->GetPlayerContainer())
						{
							FIndex2D index;
							index.x = SelectedItem.x;
							index.y = SelectedItem.y;
							Container->DropItem(*Item, index);
						}
					}
				}
			}
		}
	}
}

void UContainerWidget::MoveSelectedItem() {

}

void UContainerWidget::SelectLeftItem() {

}

void UContainerWidget::SelectRightItem() {

}

void UContainerWidget::SelectUpperItem() {

}

void UContainerWidget::SelectLowerItem() {

}


