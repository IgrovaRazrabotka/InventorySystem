// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InventorySystem/UI/ContainerWidget.h"
#include "MyGameInstance.h"
#include "InventorySystem/Components/ItemAComp.h"

#include "ContainerAComp.generated.h"


UCLASS( abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INCOMESYSTEM_API UContainerAComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UContainerAComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class UMyGameInstance *GameInstance;

	UPROPERTY()
	UItemArray3D* Items;

	UPROPERTY(EditAnywhere)
	TArray<UClass*> StartupItems;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual bool SetContent();
	virtual bool MoveItem(UItemAComp& itemToMove, FIndex2D index);
	virtual bool AddItem(UItemAComp& object);
	virtual bool RemoveItem(UItemAComp& object, FIndex2D index);
	virtual UContainerWidget* GetContainerWidget();
	virtual UItemArray3D* GetItems();
	virtual UObject* GetContainerOwner();
	virtual bool InteractFromUI(UItemAComp& clickedItem, FIndex2D index);
	virtual bool DropItem(UItemAComp& clickedItem, FIndex2D index);
	virtual bool UseItem(UItemAComp& clickedItem, FIndex2D index);
	virtual FIndex2D SeekSlotToPlaceItem(UItemAComp& itemToPlace);

};
