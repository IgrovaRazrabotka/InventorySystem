// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemArray3D.generated.h"

USTRUCT(BlueprintType)
struct FIndex2D {
	GENERATED_BODY()

	int32 x = -1;
	int32 y = -1;

	bool IsValid() 
	{
		return x >= 0 && y >= 0;
	}
};


UCLASS()
class INCOMESYSTEM_API UItemArray1D : public UObject
{
	GENERATED_BODY()
public:
	UItemArray1D() {};
	UPROPERTY()
	TArray<class UItemAComp*> dimension;
};

UCLASS()
class INCOMESYSTEM_API UItemArray2D : public UObject
{
	GENERATED_BODY()
public:
	UItemArray2D() {};
	
	bool Init(int32 SecondDimensionSize)
	{
		for (int32 y = 0; y < SecondDimensionSize; y++) {
			dimension.Add(NewObject<UItemArray1D>(this));
		}
		return true;
	}
	
	UPROPERTY()
	TArray<UItemArray1D*> dimension;
};

UCLASS()
class INCOMESYSTEM_API UItemArray3D : public UObject
{
	GENERATED_BODY()
public:
	UItemArray3D() {};
	
	bool Init(int32 ThirdDimensionSize, int32 SecondDimensionSize) 
	{
		for (int32 x = 0; x < ThirdDimensionSize; x++) {
			dimension.Add(NewObject<UItemArray2D>(this));
		}
		for (int32 x = 0; x < ThirdDimensionSize; x++) {
				dimension[x]->Init(SecondDimensionSize);
		}
		return true;
	}
	
	UPROPERTY()
	TArray<UItemArray2D*> dimension;
};

