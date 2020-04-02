// Fill out your copyright notice in the Description page of Project Settings.

#include "ContainerInteractableAComp.h"
#include "MyGameInstance.h"
#include "ContainerAComp.h"
#include "InventorySystemManager.h"

UContainerInteractableAComp::UContainerInteractableAComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UContainerInteractableAComp::BeginPlay()
{
	Super::BeginPlay();

	ContainerComp = Cast<UContainerAComp>(GetOwner()->GetComponentByClass(UContainerAComp::StaticClass()));
	verifyf(ContainerComp != nullptr, TEXT("An 'ContainerInteractableAComp' requires a component derived by 'ContainerAComp' to operate properly."));

}

bool UContainerInteractableAComp::Interact(AActor * CallerActor)
{
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetOwner()->GetGameInstance());
	if (GameInstance) {
		if(UContainerWidget* NonPlayerContainerWidget = GameInstance->InventorySystemManager->GetNonPlayerContainerWidget())
		{
			if (ContainerComp) {

				float Dist = FVector::Distance(CallerActor->GetActorLocation(), GetOwner()->GetActorLocation());
				if (Dist <= GameInstance->InventorySystemManager->InteractionDistance) {

					GameInstance->InventorySystemManager->EnterInventoryAndLootState(ContainerComp ,NonPlayerContainerWidget);
					NonPlayerContainerWidget->Open(ContainerComp);
				}
			}
		}
	}
	return false;
}