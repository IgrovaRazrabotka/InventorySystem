// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableAComp.h"
#include "ContainerInteractableAComp.generated.h"

/**
 * 
 */
UCLASS()
class INCOMESYSTEM_API UContainerInteractableAComp : public UInteractableAComp
{
	GENERATED_BODY()
public:

	UContainerInteractableAComp();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class UContainerAComp* ContainerComp = nullptr;

public:
	virtual bool Interact(AActor* CallerActor) override;
};
