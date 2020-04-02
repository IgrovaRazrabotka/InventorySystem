// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IncomeSystemGameMode.generated.h"

UCLASS(minimalapi)
class AIncomeSystemGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AIncomeSystemGameMode();

	//Here we make sure we create the UI / user interface before anything else may ever wish to access it.
	virtual void InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage);

	//This is just the tick/update function
	virtual void Tick(float DeltaTime);
};



