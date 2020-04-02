// Fill out your copyright notice in the Description page of Project Settings.

//This is the main widget/window class. It is needed to have all widgets rooted to a single widget to be able to have easy transition of focus between widgets
//If you attempt to have them separate you will not be able to click on one widget if you have clicked on another.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayGroupWidget.generated.h"

/**
 * 
 */
class UUserWidget;
class UContainerWidget;
class UTooltipWindowWidget;

UCLASS()
class INCOMESYSTEM_API UGameplayGroupWidget : public UUserWidget
{
	GENERATED_BODY()

	//This is the constructor. Here we find all the needed widget classes in the needed directories
	UGameplayGroupWidget(const FObjectInitializer& ObjectInitializer);
private:

	//This is the class of the player container widget. It can be in any place and any form. It is loaded from a specific location/path and name.
	UPROPERTY()
	TSubclassOf<UUserWidget> PlayerContainerWidgetClass;
	//This is the class of the non-player container widget. It can be in any place and any form. It is loaded from a specific location/path and name.
	UPROPERTY()
	TSubclassOf<UUserWidget> NonPlayerContainerWidgetClass;

	UPROPERTY()
	TSubclassOf<UUserWidget> TooltipWindowWidgetClass;

	//This is the actual instance / widget of the PlayerContainerWidgetClass class
	UPROPERTY()
	UContainerWidget* PlayerContainerWidget;
	//This is the actual instance / widget of the NonPlayerContainerWidgetClass class
	UPROPERTY()
	UContainerWidget* NonPlayerContainerWidget;

	UPROPERTY()
	UTooltipWindowWidget* TooltipWindowWidget;
public:
	//Here we construct the actual widgets that we found in the UGameplayGroupWidget constructor
	//This method has to call Super::Initialize as well as any method that overrides it.
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	virtual bool Initialize();
	//virtual bool Initialize_Implementation();
};
