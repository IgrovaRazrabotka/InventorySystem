// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemInteractableAComp.h"
#include "ItemAComp.h"
#include "GameFramework/Actor.h"
#include "ContainerAComp.h"
#include "InventorySystemManager.h"

UItemInteractableAComp::UItemInteractableAComp() {
	
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UItemInteractableAComp::BeginPlay()
{
	Super::BeginPlay();

	ItemComp = Cast<UItemAComp>(GetOwner()->GetComponentByClass(UItemAComp::StaticClass()));
	verifyf(ItemComp != nullptr, TEXT("An 'ItemInteractableAComp' requires a component derived by 'ItemAComp' to operate properly."));

}

bool UItemInteractableAComp::Interact(AActor* CallerActor) {

	if(UMyGameInstance* Gi = Cast<UMyGameInstance>(GetWorld()->GetGameInstance())) 
	{
		if (CallerActor) {
			UContainerAComp* Container = Cast<UContainerAComp>(CallerActor->GetComponentByClass(UContainerAComp::StaticClass()));
			if (Container) {

				float Dist = FVector::Distance(CallerActor->GetActorLocation(), GetOwner()->GetActorLocation());

				if (Dist <= Gi->InventorySystemManager->InteractionDistance) {
				
					if (UItemAComp* ThisItem = Cast<UItemAComp>(GetOwner()->GetComponentByClass(UItemAComp::StaticClass())))
					{
						if (Container->AddItem(*ThisItem)) {

							ThisItem->DeactivateItem();
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}