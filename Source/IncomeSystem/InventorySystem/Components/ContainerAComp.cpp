// Fill out your copyright notice in the Description page of Project Settings.

#include "ContainerAComp.h"
#include "InventorySystem/Utility/ItemFactory.h"
#include "InventorySystem/Components/ItemAComp.h"
#include "InventorySystemManager.h"

// Sets default values for this component's properties
UContainerAComp::UContainerAComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UContainerAComp::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetWorld())
	{
		if (UMyGameInstance* Gi = World->GetGameInstance<UMyGameInstance>())
		{
			GameInstance = Gi;
			Items = NewObject<UItemArray3D>(this, TEXT("ItemsArray"));
			Items->Init(Gi->InventorySystemManager->GetGlobalContainerSlotsPerRow(), Gi->InventorySystemManager->GetGlobalContainerSlotsPerColumn());

			SetContent();//this just adds one apple to the crate
		}
	}
	
}


// Called every frame
void UContainerAComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Add default functionality here for any IContainer_Interface functions that are not pure virtual.

bool UContainerAComp::SetContent()
{
	if (UItemFactory* ItemFactory = GameInstance->InventorySystemManager->GetItemFactory())
	{
		for(int i = 0; i < StartupItems.Num(); i++) {

			  if (StartupItems[i] != nullptr) {
				  if (AActor* ActorToSpawn = Cast<AActor>(StartupItems[i]->ClassDefaultObject)) {
					if (UItemAComp* ItemComp = Cast<UItemAComp>(ActorToSpawn->GetComponentByClass(UItemAComp::StaticClass()))) {

						FIndex2D FoundPlace = SeekSlotToPlaceItem(*ItemComp);
						if (FoundPlace.IsValid()) {

							if (UObject* SpawnedItem = ItemFactory->SpawnItem(StartupItems[i]))
							{
								if (UItemAComp* Item = Cast<UItemAComp>(Cast<AActor>(SpawnedItem)->GetComponentByClass(UItemAComp::StaticClass())))
								{
									Item->DeactivateItem();

									if (UContainerWidget* Widget = GetContainerWidget())
									{
										UItemArray3D* tmpItems = GetItems();
										tmpItems->dimension[FoundPlace.x]->dimension[FoundPlace.y]->dimension.Add(Item);
										Widget->Update(tmpItems);
									}
								}
							}
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool UContainerAComp::MoveItem(UItemAComp& itemToMove, FIndex2D index)
{
	//This is an implementation for moving items from a common widget window to the playr window, the playr to common container implementation
	// is overriden in the player character file
	if (UWorld* World = GetWorld())
	{
		if (UContainerAComp* PlayerContainer = GameInstance->InventorySystemManager->GetPlayerContainer())
		{
			if (PlayerContainer->AddItem(itemToMove))
			{
				if (!RemoveItem(itemToMove, index))
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
	return false;
}

bool UContainerAComp::AddItem(UItemAComp& object) {

	if (UContainerWidget* Widget = GetContainerWidget())
	{
		UItemArray3D* LocalItems = GetItems();
		FIndex2D FoundPlace = SeekSlotToPlaceItem(object);
		if (FoundPlace.IsValid()) {

			Items->dimension[FoundPlace.x]->dimension[FoundPlace.y]->dimension.Add(&object);
			Widget->Update(LocalItems);
			return true;
		}
	}
	return false;
}

UObject* UContainerAComp::GetContainerOwner() {
	return GetOwner();
}

bool UContainerAComp::InteractFromUI(UItemAComp& clickedItem, FIndex2D index) {
	
	if (MoveItem(clickedItem, index))
	{
		return true;
	}
	return false;
	
}

bool UContainerAComp::RemoveItem(UItemAComp& object, FIndex2D index) {

	if (index.IsValid()) {

		if (UContainerWidget* Widget = GetContainerWidget())
		{
			UItemArray3D* LocalItems = GetItems();
			if (LocalItems) {
				int32 Rem = LocalItems->dimension[index.x]->dimension[index.y]->dimension.RemoveSingle(&object);
				if (Rem == 1) {
					Widget->Update(LocalItems);
					return true;
				}
			}
		}
	}
	return false;
}

bool UContainerAComp::DropItem(UItemAComp& clickedItem, FIndex2D index) {
	
	if (RemoveItem(clickedItem, index)) {

		AActor* Actor = clickedItem.GetOwner();
		AActor* User = GetOwner();
		if (User) {
			clickedItem.ActivateItem();
			Actor->SetActorLocation(User->GetActorLocation());
			return true;
		}
	}
	return false;
}

bool UContainerAComp::UseItem(UItemAComp& clickedItem, FIndex2D index) {
	
	if (RemoveItem(clickedItem, index)) {

		AActor* Actor = clickedItem.GetOwner();
		AActor* User = GetOwner();
		if (User) {
			clickedItem.UseItem(User, index);
			return true;
		}
	}
	return false;
}

FIndex2D UContainerAComp::SeekSlotToPlaceItem(UItemAComp& itemToPlace)
{
	
	UItemArray3D* items = GetItems();
	FIndex2D firstFree;
	if (items) {

		bool bFirstFreeFound = false;

		UObject* thisObj = GetOwner();
		UWorld* world = thisObj->GetWorld();
		if (world) {
			UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
			if (mgi) {
				for (int32 y = 0; y < mgi->InventorySystemManager->GetGlobalContainerSlotsPerColumn(); y++) {

					for (int32 x = 0; x < mgi->InventorySystemManager->GetGlobalContainerSlotsPerRow(); x++) {
						//Iterate over all the elements of the 2D grid

						if (items->dimension[x]->dimension[y]->dimension.Num() > 0) {//Ask if there are items on the stack of this grid element([x,y])

							UClass* identity1 = (*items->dimension[x]->dimension[y]->dimension.GetData())->GetOwner()->GetClass();
							UClass* identity2 = itemToPlace.GetOwner()->GetClass();
							if (identity1 && identity2)
							{
								if (identity1 == identity2) //If yes: are these stack items the same as the items we want to place
								{
									// If yes: get to the interface type of the first object in the stack and check the if there is free room for one more item
									if(UItemAComp* element = *items->dimension[x]->dimension[y]->dimension.GetData())
									{
										if (element->HasFreeSpaceInStack(items->dimension[x]->dimension[y]->dimension)) {
											//If there IS space for one more item: return the [x,y] coordinates of the stack
											FIndex2D index;
											index.x = x;
											index.y = y;
											return index;
										}
									}
								}
							}
						}
						else {
							//In this case cache the first free slot so we can use it if no free stack is found
							if (!bFirstFreeFound) {
								bFirstFreeFound = true;
								firstFree.x = x;
								firstFree.y = y;
							}
						}
					}
				}
			}
		}
	}
	//Otherwise return the first free slot found OR the default invalid stack [-1,-1], which means there is no free space in the CONTAINER
	return firstFree;
}

UContainerWidget* UContainerAComp::GetContainerWidget() {

	if(UMyGameInstance* Gi = Cast<UMyGameInstance>(GetOwner()->GetGameInstance()))
	{
		return Gi->InventorySystemManager->GetNonPlayerContainerWidget();
	}
	return nullptr;
}

UItemArray3D* UContainerAComp::GetItems() {

	return Items;
}

