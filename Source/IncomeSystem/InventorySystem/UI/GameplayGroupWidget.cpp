// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplayGroupWidget.h"
#include "CanvasPanel.h"
#include "WidgetTree.h"
#include "MyGameInstance.h"
#include "ContainerWidget.h"
#include "ConstructorHelpers.h"
#include "NamedSlot.h"
#include "TooltipWindowWidget.h"
#include "InventorySystemManager.h"

UGameplayGroupWidget::UGameplayGroupWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerContainerWidgetClassFinder(TEXT("/Game/UI/UI_PlayerContainer"));
	if (PlayerContainerWidgetClassFinder.Succeeded()) {
		PlayerContainerWidgetClass = PlayerContainerWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> NonPlayerContainerWidgetClassFinder(TEXT("/Game/UI/UI_NonPlayerContainer"));
	if (NonPlayerContainerWidgetClassFinder.Succeeded()) {
		NonPlayerContainerWidgetClass = NonPlayerContainerWidgetClassFinder.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> TooltipWindowWidgetClassFinder(TEXT("/Game/UI/UI_TooltipWindow"));
	if (TooltipWindowWidgetClassFinder.Succeeded()) {
		TooltipWindowWidgetClass = TooltipWindowWidgetClassFinder.Class;
	}
}

bool UGameplayGroupWidget::Initialize() {

	Super::Initialize();

	UWorld* world = GetWorld();
	if (world) {
		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
		if (mgi) {

			UNamedSlot* playerSlot = Cast<UNamedSlot>(GetWidgetFromName(FName(TEXT("PlayerContainerWidgetSlot"))));
			if (playerSlot) {

				PlayerContainerWidget = WidgetTree->ConstructWidget<UContainerWidget>
					(PlayerContainerWidgetClass, FName(TEXT("PlayerContainerWidget")));
				playerSlot->AddChild(PlayerContainerWidget);
				mgi->InventorySystemManager->SetPlayerContainerWidget(*Cast<UPlayerContainerWidget>(PlayerContainerWidget));
			}

			UNamedSlot* nonPlayerSlot = Cast<UNamedSlot>(GetWidgetFromName(FName(TEXT("NonPlayerContainerWidgetSlot"))));
			if (nonPlayerSlot) {

				NonPlayerContainerWidget = WidgetTree->ConstructWidget<UContainerWidget>
					(NonPlayerContainerWidgetClass, FName(TEXT("NonPlayerContainerWidget")));
				nonPlayerSlot->AddChild(NonPlayerContainerWidget);
			}

			UNamedSlot* tooltipWindowSlot = Cast<UNamedSlot>(GetWidgetFromName(FName(TEXT("TooltipWindowWidgetSlot"))));
			if (tooltipWindowSlot) {

				TooltipWindowWidget = WidgetTree->ConstructWidget<UTooltipWindowWidget>
					(TooltipWindowWidgetClass, FName(TEXT("TooltipWindowWidget")));
				tooltipWindowSlot->AddChild(TooltipWindowWidget);
			}
		}
	}
	return true;
}

