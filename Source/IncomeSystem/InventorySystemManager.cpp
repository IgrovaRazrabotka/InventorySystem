// Fill out your copyright notice in the Description page of Project Settings.

#include "InventorySystemManager.h"
#include "ConstructorHelpers.h"
#include "UserWidget.h"
#include "InventorySystem/UI/GameplayGroupWidget.h"
#include "InventorySystem/UI/ContainerWidget.h"
#include "CanvasPanel.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "InventorySystem/UI/ItemSlotBorder.h"
#include "Engine.h"
#include "InventorySystem/UI/TooltipWindowWidget.h"
#include "InventorySystem/Utility/ItemFactory.h"
#include "InventorySystem/Components/ContainerAComp.h"

UInventorySystemManager::UInventorySystemManager() {

	static ConstructorHelpers::FClassFinder<UUserWidget> GameplayGroupClassFinder(TEXT("/Game/UI/UI_GameplayGroup"));
	if (GameplayGroupClassFinder.Succeeded()) {
		GameplayGroupWidgetClass = GameplayGroupClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> SlotButtonClassFinder(TEXT("/Game/UI/UI_InventorySlot"));
	if (SlotButtonClassFinder.Succeeded()) {
		SlotButtonWidgetClass = SlotButtonClassFinder.Class;
	}

	//item factory
	ItemFactory = CreateDefaultSubobject<UItemFactory>(TEXT("ItemFactory"));
}

void UInventorySystemManager::EnterInventoryOnlyState(UContainerWidget* PlayerWidget, UContainerAComp* PlayerContainer) {
	InventorySystemState = EInventorySystemState::EInventoryOnly;
	PlayerWidget->Open(PlayerContainer);
	CurrentlyBrowsedContainerWidget = PlayerWidget;
	CurrentlyBrowsedContainerWidget->SelectLastSelectedItem();
}

void UInventorySystemManager::EnterInventoryAndLootState(UContainerAComp* _NonPlayerContainer, UContainerWidget* _CurrentlyBrowsedContainerWidget) {
	InventorySystemState = EInventorySystemState::EInventoryAndLoot;
	SetCurrentlyOpenNonPlayerContainer(_NonPlayerContainer);
	PlayerContainerWidget->Open(GetPlayerContainer());
	CurrentlyBrowsedContainerWidget = _CurrentlyBrowsedContainerWidget;
	CurrentlyBrowsedContainerWidget->SelectItem(0, 0);
}

void UInventorySystemManager::ExitInventoryOnlyState(UContainerWidget* PlayerWidget) {
	InventorySystemState = EInventorySystemState::EClosed;
	PlayerWidget->Close();
	CurrentlyBrowsedContainerWidget->DeselectCurrentlySelectedItem();
	CurrentlyBrowsedContainerWidget = nullptr;
}

void UInventorySystemManager::ExitInventoryAndLootState(UContainerWidget* PlayerWidget, UContainerWidget* NonPlayerWidget) {
	InventorySystemState = EInventorySystemState::EClosed;
	if (NonPlayerWidget) {
		PlayerWidget->Close();
		NonPlayerWidget->Close();
		CurrentlyBrowsedContainerWidget->DeselectCurrentlySelectedItem();
		CurrentlyBrowsedContainerWidget = nullptr;
	}
}

UContainerWidget* UInventorySystemManager::GetCurrentlyBrowsedContainerWidget() {
	return CurrentlyBrowsedContainerWidget;
}

void UInventorySystemManager::SetCurrentlyBrowsedContainerWidget(UContainerWidget& ContainerWidget) {
	CurrentlyBrowsedContainerWidget = &ContainerWidget;
}

void UInventorySystemManager::SetPlayerContainerWidget(UPlayerContainerWidget& _PlayerContainerWidget) {
	PlayerContainerWidget = &_PlayerContainerWidget;
}

UContainerWidget * UInventorySystemManager::GetPlayerContainerWidget()
{
	FName name;
	if (GameplayGroupWidget) {
		name = FName(TEXT("PlayerContainerWidget"));
		UContainerWidget* widget = Cast<UContainerWidget>(GameplayGroupWidget->GetWidgetFromName(name));
		return widget;
	}
	return nullptr;
}

UContainerWidget * UInventorySystemManager::GetNonPlayerContainerWidget() {

	if (GameplayGroupWidget) {

		UContainerWidget * widget = Cast<UContainerWidget>(GameplayGroupWidget->GetWidgetFromName(FName(TEXT("NonPlayerContainerWidget"))));
		return widget;
	}
	return nullptr;
}

UTooltipWindowWidget * UInventorySystemManager::GetTooltipWindowWidget() {

	if (GameplayGroupWidget)
	{
		UTooltipWindowWidget * widget = Cast<UTooltipWindowWidget>(GameplayGroupWidget->GetWidgetFromName(FName(TEXT("TooltipWindowWidget"))));
		return widget;
	}
	return nullptr;
}

UContainerAComp* UInventorySystemManager::GetPlayerContainer()
{
	ACharacter* Ch = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (Ch) {
		return Cast<UContainerAComp>(Ch->GetComponentByClass(UContainerAComp::StaticClass()));
	}
	return nullptr;
}

UContainerAComp* UInventorySystemManager::GetCurrentlyOpenNonPlayerContainer() {
	return CurrentlyOpenNonPlayerContainer;
}

bool UInventorySystemManager::SetPlayerContainer(UContainerAComp* container)
{
	PlayerContainer = container;
	return true;
}

bool UInventorySystemManager::SetCurrentlyOpenNonPlayerContainer(UContainerAComp* container)
{
	CurrentlyOpenNonPlayerContainer = container;
	return true;
}

int32 UInventorySystemManager::GetGlobalContainerSlotsPerRow()
{
	// X - horizontal dimension (width of the inventory window in slots)
	return 4;
}

int32 UInventorySystemManager::GetGlobalContainerSlotsPerColumn()
{
	// Y - vertical dimension (height of the inventory window in slots)
	return 4;
}

int32 UInventorySystemManager::GetGlobalContainerTotalSlotNumber()
{
	return GetGlobalContainerSlotsPerRow() * GetGlobalContainerSlotsPerColumn();
}

TSubclassOf<UUserWidget> UInventorySystemManager::GetContainerSlotClass() {
	return SlotButtonWidgetClass;
}

//input
void UInventorySystemManager::SetHoldToDrop(bool flag) {
	bHoldToDrop = flag;
}
bool UInventorySystemManager::GetHoldToDrop() {
	return bHoldToDrop;
}

//item factory
UItemFactory* UInventorySystemManager::GetItemFactory() {
	return ItemFactory;
}