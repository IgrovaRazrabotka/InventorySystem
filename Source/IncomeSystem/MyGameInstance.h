// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "MyGameInstance.generated.h"

//This is a log category - unimportant
DECLARE_LOG_CATEGORY_EXTERN(UnhandledCase, Warning, All);

/**
 * 
 */
UCLASS()
class INCOMESYSTEM_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UMyGameInstance();

private:
	
	

public:

	UPROPERTY()
	class UInventorySystemManager* InventorySystemManager = nullptr;
	
	//This method is called by the GameMode in the game Init() method to make sure the UI is created before it gets used by the various game
	//systems and the Inventory system elements. It should remain this way or in an equivalent way.
	UFUNCTION(BlueprintCallable)
	void CreateGameUI();
};
