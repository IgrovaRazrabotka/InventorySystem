// Fill out your copyright notice in the Description page of Project Settings.

#include "TooltipWindowWidget.h"
#include "WidgetTree.h"
#include "TextBlock.h"



bool UTooltipWindowWidget::Open(FString& theTooltipText) {
	
	if (GetVisibility() == ESlateVisibility::Hidden) {
		
		UTextBlock* theText = GetTooltipText();
		if (theText) {
			
			theText->SetText(FText::FromString(theTooltipText));
			SetVisibility(ESlateVisibility::Visible);
			return true;
		}
	}
	return false;
}

bool UTooltipWindowWidget::Close() {

	if (GetVisibility() == ESlateVisibility::Visible) {

		SetVisibility(ESlateVisibility::Hidden);
		return true;
	}
	return false;
}

UTextBlock* UTooltipWindowWidget::GetTooltipText() {
	
	return WidgetTree->FindWidget<UTextBlock>(TEXT("TextField"));
}

