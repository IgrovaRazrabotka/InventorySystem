// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "InventorySystem/Utility/ItemArray3D.h"
#include "ItemSlotBorder.generated.h"

/**
 * 
 */
class UItemAComp;
class UContainerAComp;

USTRUCT(BlueprintType)
struct FItemSlotBorderSettings
{
	GENERATED_BODY()

	FItemSlotBorderSettings() {};

	UPROPERTY(BlueprintReadWrite, Category = "Container reference")
	FIndex2D index;

	UPROPERTY()
	UItemAComp* ItemHeld = nullptr;
	UPROPERTY()
	UContainerAComp* ParentContainer = nullptr;

};

UCLASS(meta = (DisplayName = "Item Slot Border"))
class INCOMESYSTEM_API UItemSlotBorder : public UBorder
{
	GENERATED_BODY()

	UItemSlotBorder();

public:

	void SetContents(UItemAComp* item, FIndex2D index);

	bool IsEmpty();

	bool Init(FItemSlotBorderSettings settings);

	bool SetParentContainer(UContainerAComp* container);

	bool SetItemHeld(UItemAComp* item);

	UContainerAComp* GetParentContainer();

	UItemAComp* GetItemHeld();

protected:

	FItemSlotBorderSettings Settings;

	UPROPERTY()
	USlateBrushAsset* EmptyBrushDefaults;

	UPROPERTY()
	USlateBrushAsset* FullBrushDefaults;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
	
};
