// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySlotWidget.h"
#include "ItemSlotBorder.h"
#include "TextBlock.h"
#include "WidgetTree.h"
#include "SelectionFrame.h"

UItemSlotBorder* UInventorySlotWidget::GetItemSlotBorder() {
	
	if(WidgetTree){
		return WidgetTree->FindWidget<UItemSlotBorder>(TEXT("ItemSlotBorder"));
	}
	return nullptr;
}

UTextBlock* UInventorySlotWidget::GetStackNumber() {

	if (WidgetTree) {
		return WidgetTree->FindWidget<UTextBlock>(TEXT("StackNumber"));
	}
	return nullptr;
}

USelectionFrame* UInventorySlotWidget::GetSelectionFrame() {

	if (WidgetTree) {
		return WidgetTree->FindWidget<USelectionFrame>(TEXT("SelectionFrame"));
	}
	return nullptr;
}



