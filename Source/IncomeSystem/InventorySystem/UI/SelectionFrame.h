// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "SelectionFrame.generated.h"

/**
 * 
 */

class USlateBrushAsset;

UCLASS(meta = (DisplayName = "Selection Frame"))
class INCOMESYSTEM_API USelectionFrame : public UBorder
{
	GENERATED_BODY()

	USelectionFrame();
public:

	void SetState(bool bSelected);
protected:
	UPROPERTY()
		USlateBrushAsset* UnselectedBrushDefaults;

	UPROPERTY()
		USlateBrushAsset* SelectedBrushDefaults;

#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
