// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemFactory.h"

UItemFactory::UItemFactory()
{
}

AActor* UItemFactory::SpawnItem(UClass* ClassToSpawn)
{
	if (UWorld* world = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::ReturnNull)) {
			return world->SpawnActor(ClassToSpawn);
	}
	return nullptr;
}
