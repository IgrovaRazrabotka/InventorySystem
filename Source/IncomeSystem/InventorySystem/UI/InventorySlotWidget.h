// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
class UItemSlotBorder;
class UTextBlock;
class USelectionFrame;

UCLASS()
class INCOMESYSTEM_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UItemSlotBorder * GetItemSlotBorder();
	UTextBlock * GetStackNumber();
	USelectionFrame * GetSelectionFrame();
	
};
