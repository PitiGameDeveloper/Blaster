// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class BLASTER_API CodeUtils
{
public:

	UFUNCTION()
	static void PrintToScreen(FString Message, FColor Color = FColor::Yellow, float Duration = 15.0f);

	UFUNCTION()
	static void PrintToScreen(FText Message, FColor Color = FColor::Yellow, float Duration = 15.0f);
	
	UFUNCTION()
	static void PrintIntToScreen(int32 Value, FColor Color = FColor::Yellow, float Duration = 15.0f);

	UFUNCTION()
	static void PrintFloatToScreen(float Value, FColor Color = FColor::Yellow, float Duration = 15.0f);


};
