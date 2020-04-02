// Fill out your copyright notice in the Description page of Project Settings.

// This is a utility class that simulates work of 2D arrays with 1D arrays
#pragma once

#include "CoreMinimal.h"
#include "ItemArray3D.h"

//Traverse the array and set a 2D element by interpreting the 1D element as a 2D one.
template<typename T>
static void SetElement2D(int32 row, int32 column, int32 slotsPerRow, TArray<T>& Array, T value) {

	Array[(slotsPerRow * column) + row] = value;
}

//Traverse the array and get a 2D element by interpreting the 1D element as a 2D one
template<typename T>
static T GetPtrToElement2D(int32 row, int32 column, int32 slotsPerRow, TArray<T>& Array) {
	
	int32 index = (slotsPerRow * column) + row;
	if (index >= 0 && index < Array.Num()) {
		return Array[(slotsPerRow * column) + row];
	}
	return nullptr;
}

//Check if a set of a row and a column make up a valid 2D array coordinate.
static bool IsValidIndex2D(int32 row, int32 column, int32 slotsPerRow, int32 maxCapacity) {
	
	int32 index = (slotsPerRow * column) + row;
	return index >= 0 && index < maxCapacity;
}

//Converts a 1D array index to a 2D array index thanks to the input parameters.
static void From1DTo2D(int32 index, int32 slotsPerRow, int32& outRow, int32& outColumn) {

	outRow = index % slotsPerRow;
	outColumn = index / slotsPerRow;
}

//Converts a 2D array index to a 1D array index thanks to the input parameters.
static int32 From2DTo1D(int32 /*horizontal*/row, int32 /*vertical*/column, int32 slotsPerRow) {

	return (slotsPerRow * column) + row;

}
