// Fill out your copyright notice in the Description page of Project Settings.

#include "TooltipAComp.h"
#include "MyGameInstance.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ScriptDelegates.h"
#include "InventorySystemManager.h"

// Sets default values for this component's properties
UTooltipAComp::UTooltipAComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


	// ...
}


// Called when the game starts
void UTooltipAComp::BeginPlay()
{
	Super::BeginPlay();

	if (UWorld* World = GetOwner()->GetWorld()) {
		if (UMyGameInstance* Gi = World->GetGameInstance<UMyGameInstance>()) {

			TooltipWindowWidget = Gi->InventorySystemManager->GetTooltipWindowWidget();
		}
	}

	verifyf(TooltipWindowWidget != nullptr, TEXT("ERROR: TooltipWIndowWidget is null."));

	UStaticMeshComponent* Mesh  = Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	
	verifyf(Mesh != nullptr, TEXT("ERROR: Mesh of TooltipWIndowWidget is null."));

	TScriptDelegate<> DelegateBeginOver;
	DelegateBeginOver.BindUFunction(this, "OnHoverBegin");
	Mesh->OnBeginCursorOver.Clear();
	Mesh->OnBeginCursorOver.Add(DelegateBeginOver);

	TScriptDelegate<> DelegateEndOver;
	DelegateEndOver.BindUFunction(this, "OnHoverEnd");
	Mesh->OnEndCursorOver.Clear();
	Mesh->OnEndCursorOver.Add(DelegateEndOver);
}


// Called every frame
void UTooltipAComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTooltipAComp::OnHoverBegin() {

	if (TooltipWindowWidget) TooltipWindowWidget->Open(TooltipText);
}

void UTooltipAComp::OnHoverEnd() {

	if (TooltipWindowWidget) TooltipWindowWidget->Close();
}