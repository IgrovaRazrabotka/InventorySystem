// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem/Components/ItemAComp.h"
#include "InventorySystem/Components/TooltipAComp.h"
#include "InventorySystem/Components/ItemInteractableAComp.h"
#include "MyGameInstance.h"
#include "WorldItemBase.generated.h"

UCLASS(abstract)
class INCOMESYSTEM_API AWorldItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent * Mesh;
	
	UPROPERTY(EditAnywhere)
	UItemInteractableAComp* ItemInterComp;
	
	UPROPERTY(EditAnywhere)
	UItemAComp* ItemComp;

	UPROPERTY(EditAnywhere)
	UTooltipAComp* TooltipComp;
	
};
