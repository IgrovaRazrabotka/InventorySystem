// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldContainerBase.h"
#include "Engine.h"
#include "MyGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "IncomeSystemPlayerController.h"
#include "InventorySystemManager.h"


// Sets default values
AWorldContainerBase::AWorldContainerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Mesh")));

	ContainerInterComp = CreateDefaultSubobject<UContainerInteractableAComp>(TEXT("Container Interactable Component"));
	ContainerComp = CreateDefaultSubobject<UItemContainerAComp>(TEXT("Item Container Component"));

}

// Called when the game starts or when spawned
void AWorldContainerBase::BeginPlay()
{
	Super::BeginPlay();	
	
	UWorld* world = GetWorld();
	if (world) {
		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();

		if (mgi) {
			UItemArray3D* Items = ContainerComp->GetItems();
			Items = NewObject<UItemArray3D>(this, TEXT("ItemsArray"));
			Items->Init(mgi->InventorySystemManager->GetGlobalContainerSlotsPerRow(), mgi->InventorySystemManager->GetGlobalContainerSlotsPerColumn());
		}
	}
}

void AWorldContainerBase::PostActorCreated() {
	
	Super::PostActorCreated();
}

// Called every frame
void AWorldContainerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}