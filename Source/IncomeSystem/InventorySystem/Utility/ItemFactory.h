// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "MyGameInstance.h"
#include "ItemFactory.generated.h"

/**
 * 
 */
UCLASS()
class INCOMESYSTEM_API UItemFactory : public UObject
{
	GENERATED_BODY()
public:
	UItemFactory();

	AActor* SpawnItem(UClass* ClassToSpawn);
};
