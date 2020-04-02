// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ContainerWidget.h"
#include "PlayerContainerWidget.generated.h"

/**
 * 
 */
UCLASS()
class INCOMESYSTEM_API UPlayerContainerWidget : public UContainerWidget
{
	GENERATED_BODY()

public:

	virtual bool Open(UContainerAComp* container) override;

	virtual bool Close() override;

	virtual void SelectLeftItem() override;
	virtual void SelectRightItem() override;
	virtual void SelectUpperItem() override;
	virtual void SelectLowerItem() override;

	virtual void MoveSelectedItem() override;
};
