// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContainerAComp.h"
#include "ItemAComp.h"
#include "PlayerItemContainerAComp.generated.h"

/**
 * 
 */
UCLASS()
class INCOMESYSTEM_API UPlayerItemContainerAComp : public UContainerAComp
{
	GENERATED_BODY()
public:
	UPlayerItemContainerAComp();
public:

	virtual UContainerWidget* GetContainerWidget() override;
	virtual bool MoveItem(UItemAComp& itemToMove, FIndex2D index) override;
	virtual bool InteractFromUI(UItemAComp& clickedItem, FIndex2D index) override;
	
};
