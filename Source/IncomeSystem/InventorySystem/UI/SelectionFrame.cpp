// Fill out your copyright notice in the Description page of Project Settings.

#include "SelectionFrame.h"
#include "ConstructorHelpers.h"
#include "Slate/SlateBrushAsset.h"

#define LOCTEXT_NAMESPACE "UMG"

USelectionFrame::USelectionFrame() {

	static ConstructorHelpers::FObjectFinder<USlateBrushAsset> UnselectedBrushFinder(TEXT("/Game/UI/SelectionFrameEmptyBrush"));
	static ConstructorHelpers::FObjectFinder<USlateBrushAsset> SelectedBrushFinder(TEXT("/Game/UI/SelectionFrameFullBrush"));

	UnselectedBrushDefaults = UnselectedBrushFinder.Object;
	SelectedBrushDefaults = SelectedBrushFinder.Object;
}

void USelectionFrame::SetState(bool bSelected) {

	if (UnselectedBrushDefaults != nullptr && SelectedBrushDefaults != nullptr) {

		if (bSelected) {
			SetBrush(SelectedBrushDefaults->Brush);
		}
		else {
			SetBrush(UnselectedBrushDefaults->Brush);
		}
	}
}

#if WITH_EDITOR
const FText USelectionFrame::GetPaletteCategory() {

	return LOCTEXT("", "CUSTOM");
}
#endif

#undef LOCTEXT_NAMESPACE