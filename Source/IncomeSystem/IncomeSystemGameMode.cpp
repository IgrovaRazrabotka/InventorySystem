// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "IncomeSystemGameMode.h"
#include "IncomeSystemPlayerController.h"
#include "IncomeSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MyGameInstance.h"
#include "InventorySystemManager.h"

AIncomeSystemGameMode::AIncomeSystemGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

	// Use our custom PlayerController class
	PlayerControllerClass = AIncomeSystemPlayerController::StaticClass();

	// Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AIncomeSystemGameMode::InitGame(const FString & MapName, const FString & Options, FString & ErrorMessage) {

	UWorld* world = GetWorld();
	if (world) {
		UMyGameInstance* mgi = world->GetGameInstance<UMyGameInstance>();
		if (mgi) {
			
			mgi->CreateGameUI();
		}
	}

	Super::InitGame(MapName, Options, ErrorMessage);

}

void AIncomeSystemGameMode::Tick(float DeltaTime) {

}

