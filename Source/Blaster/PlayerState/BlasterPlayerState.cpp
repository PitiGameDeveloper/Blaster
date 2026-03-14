// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterPlayerState.h"
#include "Blaster/Character/BlasterCharacter.h"
#include "Blaster/PlayerController/BlasterPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "Blaster/CodeUtils/CodeUtils.h"


void ABlasterPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(ABlasterPlayerState, Defeats);
}


void ABlasterPlayerState::AddToScore(float ScoreAmount)
{
	SetScore(GetScore() + ScoreAmount);

	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void ABlasterPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller)
		{
			Controller->SetHUDScore(GetScore());
		}
	}
}

void ABlasterPlayerState::AddToDefeats(int32 DefeatsAmount)
{
	Defeats += DefeatsAmount;

	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	/*if (Character == nullptr)
		Character = Cast<ABlasterCharacter>(GetPawn());
	else
		Character = Character;*/

	if (Character)
	{
		Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		/*if (Controller == nullptr)
			Controller = Cast<ABlasterPlayerController>(Character->Controller);
		else
			Controller = Controller;*/

		if (Controller)
		{
			CodeUtils::PrintToScreen("PlayerStatePrint");
			CodeUtils::PrintIntToScreen(Defeats, FColor::Green);
			Controller->SetHUDDefeats(Defeats);

		}
	}
}

void ABlasterPlayerState::OnRep_Defeats()
{
	Character = Character == nullptr ? Cast<ABlasterCharacter>(GetPawn()) : Character;
	if (Character)
	{
		//Controller = Controller == nullptr ? Cast<ABlasterPlayerController>(Character->Controller) : Controller;
		if (Controller == nullptr)
			Cast<ABlasterPlayerController>(Character->Controller);
		else
			Controller = Controller;

		if (Controller)
		{
			CodeUtils::PrintToScreen("OnRepPrint");
			CodeUtils::PrintIntToScreen(Defeats, FColor::Blue);
			Controller->SetHUDDefeats(Defeats);
		}
	}
}
