// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableAComp.h"
#include "ItemInteractableAComp.generated.h"

/**
 * 
 */
UCLASS()
class INCOMESYSTEM_API UItemInteractableAComp : public UInteractableAComp
{
	GENERATED_BODY()
public:
	UItemInteractableAComp();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class UItemAComp* ItemComp = nullptr;

public:
	virtual bool Interact(AActor* CallerActor) override;
};
