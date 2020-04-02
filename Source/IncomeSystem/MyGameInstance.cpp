// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "ConstructorHelpers.h"
#include "InventorySystemManager.h"
#include "UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

DEFINE_LOG_CATEGORY(UnhandledCase);

UMyGameInstance::UMyGameInstance() {

	InventorySystemManager = NewObject<UInventorySystemManager>(this, FName(TEXT("InventorySystemManager")));
	verifyf(InventorySystemManager != nullptr, TEXT("Error: Inventory System Manager is null. Check the game instance."));
}

void UMyGameInstance::CreateGameUI() {

	InventorySystemManager->GameplayGroupWidget = CreateWidget<UGameplayGroupWidget>(this, InventorySystemManager->GameplayGroupWidgetClass);
	if (InventorySystemManager->GameplayGroupWidget) {
		InventorySystemManager->GameplayGroupWidget->AddToViewport();
	}
}


