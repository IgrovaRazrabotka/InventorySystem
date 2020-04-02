// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventorySystem/Components/ContainerInteractableAComp.h"
#include "InventorySystem/Components/ItemContainerAComp.h"
#include "WorldContainerBase.generated.h"

UCLASS(abstract)
class INCOMESYSTEM_API AWorldContainerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldContainerBase();

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	UContainerInteractableAComp* ContainerInterComp;

	UPROPERTY(EditAnywhere)
	UItemContainerAComp* ContainerComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostActorCreated() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};
