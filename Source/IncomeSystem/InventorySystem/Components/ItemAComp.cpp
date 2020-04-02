// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemAComp.h"
#include "Engine.h"
#include "ContainerAComp.h"

// Sets default values for this component's properties
UItemAComp::UItemAComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UItemAComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItemAComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* UItemAComp::GetItemActorRepresentation() {

	return GetOwner();
}
 
class UStaticMesh* UItemAComp::GetItemMesh() {
	if (UStaticMeshComponent* Mesh = GetItemMeshComponent()) {
		return Mesh->GetStaticMesh();
	}
	return nullptr;
}
 
class UStaticMeshComponent* UItemAComp::GetItemMeshComponent() {

	return Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
}

class UTexture2D* UItemAComp::GetItemIcon() {
	return ItemIcon;
}

FString UItemAComp::GetItemDescription() {
	return ItemDescription;
}

bool UItemAComp::UseItem(AActor* User, FIndex2D Index) {

	// code can be added here to create custom behaviour when using the item
	if (User) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Item used!"));
			UContainerAComp* Container = Cast<UContainerAComp>(User->GetComponentByClass(UContainerAComp::StaticClass()));
			if (Container != nullptr) {
				Container->RemoveItem(*this, Index);
				GetOwner()->Destroy();
				return true;
			}
		}
	}
	return false;
}

bool UItemAComp::HasFreeSpaceInStack(TArray<UItemAComp*>& stack) {
	
	return stack.Num() < GetMaxStackSize();
}

int32 UItemAComp::GetMaxStackSize() {

	return MaxStackSize;
}

bool UItemAComp::ActivateItem() {

	UStaticMeshComponent* Comp = GetItemMeshComponent();
	if (Comp) {
		Comp->SetSimulatePhysics(true);
	}

	AActor* Actor = GetItemActorRepresentation();
	if (Actor) {
		Actor->SetActorHiddenInGame(false);
		Actor->SetActorEnableCollision(true);
	}

	return true;
}

bool UItemAComp::DeactivateItem() {

	UStaticMeshComponent* Comp = GetItemMeshComponent();
	if (Comp) {
		Comp->SetSimulatePhysics(false);
	}

	AActor* Actor = GetItemActorRepresentation();
	if (Actor) {
		Actor->SetActorHiddenInGame(true);
		Actor->SetActorEnableCollision(false);
	}

	return true;
}
