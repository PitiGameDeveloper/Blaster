// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeUtils.h"
#include "Engine/Engine.h"

void CodeUtils::PrintToScreen(FString Message, FColor Color, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message);
	}
}

void CodeUtils::PrintToScreen(FText Message, FColor Color, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, Message.ToString());
	}
}

void CodeUtils::PrintIntToScreen(int32 Value, FColor Color, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FString::Printf(TEXT("Int Value: %d"), Value));
	}
}

void CodeUtils::PrintFloatToScreen(float Value, FColor Color, float Duration)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, FString::Printf(TEXT("Float Value: %f"), Value));
	}
}
