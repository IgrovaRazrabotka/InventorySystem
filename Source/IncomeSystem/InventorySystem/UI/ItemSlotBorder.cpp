// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemSlotBorder.h"
#include "ConstructorHelpers.h"
#include "SButton.h"
#include "WidgetTree.h"
#include "MyGameInstance.h"
#include "TooltipWindowWidget.h"
#include "Engine.h"
#include "InventorySystem/Components/ItemAComp.h"
#include "InventorySystem/Components/ContainerAComp.h"
#include "InventorySystemManager.h"
#include "Slate/SlateBrushAsset.h"

#define LOCTEXT_NAMESPACE "UMG"

UItemSlotBorder::UItemSlotBorder() {

	static ConstructorHelpers::FObjectFinder<USlateBrushAsset> EmptyBrushFinder(TEXT("/Game/UI/ItemSlotBorderEmptyBrush"));
	static ConstructorHelpers::FObjectFinder<USlateBrushAsset> FullBrushFinder(TEXT("/Game/UI/ItemSlotBorderFullBrush"));

	EmptyBrushDefaults = EmptyBrushFinder.Object;
	FullBrushDefaults = FullBrushFinder.Object;

	SetContents(nullptr,FIndex2D());
}

void UItemSlotBorder::SetContents(UItemAComp* item, FIndex2D index)
{
	if (item == nullptr) {
		// set empty style
		if (EmptyBrushDefaults) {
			SetBrush(EmptyBrushDefaults->Brush);
			SetItemHeld(nullptr);
			Settings.index = FIndex2D();
		}
	}
	else {
		if (FullBrushDefaults) {
			if (item) {
				SetItemHeld(item);
				Settings.index.x = index.x;
				Settings.index.y = index.y;
				UTexture2D* Icon = item->GetItemIcon();
				if (Icon) {
					FSlateBrush NewBrush = FullBrushDefaults->Brush;
					NewBrush.SetResourceObject(Cast<UObject>(Icon));
					SetBrush(NewBrush);
				}else{
					SetBrush(FullBrushDefaults->Brush);
				}
			}
		}
	}
}
bool UItemSlotBorder::IsEmpty()
{
	return (Settings.ItemHeld != nullptr)?true:false;
}
bool UItemSlotBorder::Init(FItemSlotBorderSettings settings)
{
	SetParentContainer(settings.ParentContainer);
	SetItemHeld(settings.ItemHeld);
	return true;
}


bool UItemSlotBorder::SetParentContainer(UContainerAComp* container) {
	Settings.ParentContainer = container;
	return true;
}

bool UItemSlotBorder::SetItemHeld(UItemAComp* item)
{
	Settings.ItemHeld = item;
	//Simulate removal and addition of a new SlotButton as we reuse them.

	UWorld* world = GetWorld();
	if (world) {
		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
		if (mgi) {

		}
	}
	
	return true;
}

UContainerAComp* UItemSlotBorder::GetParentContainer() {
	return Settings.ParentContainer;
}

UItemAComp* UItemSlotBorder::GetItemHeld() {
	return Settings.ItemHeld;
}

#if WITH_EDITOR
const FText UItemSlotBorder::GetPaletteCategory() {

	return LOCTEXT("", "CUSTOM");
}
#endif

#undef LOCTEXT_NAMESPACE

