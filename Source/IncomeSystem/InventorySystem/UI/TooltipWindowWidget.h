// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TooltipWindowWidget.generated.h"

/**
 * 
 */
class UTextBlock;

UCLASS()
class INCOMESYSTEM_API UTooltipWindowWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Open(FString& theTooltipText);

	virtual bool Close();

	virtual UTextBlock* GetTooltipText();
	
	
};
