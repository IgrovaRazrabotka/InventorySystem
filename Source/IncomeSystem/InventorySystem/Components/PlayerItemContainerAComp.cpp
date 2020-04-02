// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerItemContainerAComp.h"
#include "Engine.h"
#include "ItemAComp.h"
#include "InventorySystemManager.h"

UPlayerItemContainerAComp::UPlayerItemContainerAComp() {

	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

UContainerWidget* UPlayerItemContainerAComp::GetContainerWidget() {

	if(UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetOwner()->GetGameInstance()))
	{
		return GameInstance->InventorySystemManager->GetPlayerContainerWidget();
	}
	return nullptr;
}

bool UPlayerItemContainerAComp::MoveItem(UItemAComp& itemToMove, FIndex2D index)
{
	UObject* thisObj = Cast<UObject>(this);
	if (thisObj) {
		UWorld* world = thisObj->GetWorld();
		if (world) {
			UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
			if (mgi) {

				UContainerAComp* nonPlayerContainer = mgi->InventorySystemManager->GetCurrentlyOpenNonPlayerContainer();
				//If the container is null then we have not clicked on an interactable world object.
				//This is set whenever a window gets opened and is set to null whenever a window is closed.
				if (nonPlayerContainer) {

					if (nonPlayerContainer->AddItem(itemToMove)) {
						if (!RemoveItem(itemToMove,index))
						{
							if (GEngine) {
								GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("Remove used on null object!")));
							}
						}
						else {
							return true;
						}
					}
					else {
						if (GEngine) {
							GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString::Printf(TEXT("AddItem returned false!")));
						}
					}
				}
			}
		}
	}
	return false;
}

bool UPlayerItemContainerAComp::InteractFromUI(UItemAComp& clickedItem, FIndex2D index)
{
	UObject* thisObj = GetOwner();
	if (thisObj) {
		UWorld* world = thisObj->GetWorld();
		if (world) {
			UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
			if (mgi) {

				if(UContainerAComp* nonPlayerContainer = mgi->InventorySystemManager->GetCurrentlyOpenNonPlayerContainer())
				//If the container is null then we have not clicked on an interactable world object.
				//This is set whenever a window gets opened and is set to null whenever a window is closed
				{
					//If the common container is open -> this means that we are transferring items
					//and so we cant use items from this container.
						MoveItem(clickedItem,index);
						return true;
				}
				//else we can use an item
				else {
					clickedItem.UseItem(GetOwner(),index);
					return true;
				}
			}
		}
	}
	return false;
}