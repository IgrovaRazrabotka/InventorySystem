// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventorySystem/UI/TooltipWindowWidget.h"

#include "TooltipAComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INCOMESYSTEM_API UTooltipAComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTooltipAComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY()
	UTooltipWindowWidget* TooltipWindowWidget = nullptr;

	UPROPERTY(EditAnywhere)
	FString TooltipText = "";

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
	virtual void OnHoverBegin();
	UFUNCTION()
	virtual void OnHoverEnd();
		
};
