// Fill out your copyright notice in the Description page of Project Settings.


#include "OverheadWidget.h"
#include "Components/TextBlock.h"

void UOverheadWidget::SetDisplayText(FString textToDisplay)
{
	if (DisplayText) 
	{
		DisplayText->SetText(FText::FromString(textToDisplay));
	}
}

void UOverheadWidget::ShowPlayerNetRole(APawn* inPawn)
{
	FString name = inPawn->GetName();
	ENetRole localRole = inPawn->GetLocalRole();
	FString role;
	switch (localRole)
	{
	case ENetRole::ROLE_Authority:
		role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		role = FString("AutonomousProxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		role = FString("SimulatedProxy");
		break;
	case ENetRole::ROLE_None:
		role = FString("None");
		break;
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Pawn: %s - role: %s"), *name, *role));
	FString localRoleString = FString::Printf(TEXT("Local role: %s"), *role);

	SetDisplayText(localRoleString);
}

void UOverheadWidget::NativeDestruct()
{
	RemoveFromParent();
	Super::NativeDestruct();
}
