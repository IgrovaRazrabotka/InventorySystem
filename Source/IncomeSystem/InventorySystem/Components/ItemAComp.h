// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "InventorySystem/Utility/ItemArray3D.h"
#include "Engine/Texture2D.h"

#include "ItemAComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INCOMESYSTEM_API UItemAComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemAComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere)
	int32 MaxStackSize = 1;
	UPROPERTY(EditAnywhere)
	UTexture2D * ItemIcon;
	UPROPERTY(EditAnywhere)
	FString ItemDescription;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual AActor* GetItemActorRepresentation();
	virtual class UStaticMesh* GetItemMesh();
	virtual class UStaticMeshComponent* GetItemMeshComponent();
	virtual class UTexture2D* GetItemIcon();
	virtual FString GetItemDescription();
	virtual bool UseItem(AActor* user, FIndex2D index);
	virtual bool HasFreeSpaceInStack(TArray<UItemAComp*>& stack);
	virtual int32 GetMaxStackSize();
	virtual bool ActivateItem();
	virtual bool DeactivateItem();

		
};
