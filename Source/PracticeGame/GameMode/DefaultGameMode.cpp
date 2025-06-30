// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultGameMode.h"
#include "../Player/Rogue/Rogue.h"
#include "../Player/Ranger/Ranger.h"
#include "../PlayerController/PlayerControllerTestGame.h"

ADefaultGameMode::ADefaultGameMode()
{
	//기본 폰 클래스 설정 : Rogue
	DefaultPawnClass = ARogue::StaticClass();

	//플레이어 컨트롤러 클래스 설정
	PlayerControllerClass = APlayerControllerTestGame::StaticClass();
}

APlayerController* ADefaultGameMode::Login(UPlayer* NewPlayer,
	ENetRole InRemoteRole, const FString& Portal,
	const FString& Options, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	APlayerController* result = Super::Login(NewPlayer, InRemoteRole,
		Portal, Options,
		UniqueId, ErrorMessage);

	UE_LOG(TestGame, Warning, TEXT("DefaultGameMode : Login Entered"));
	int32	Job = 0;
	FParse::Value(*Options, TEXT("Job="), Job);

	if (Job != 0)
	{
		EPlayerJob	PlayerJob = (EPlayerJob)Job;

		switch (PlayerJob)
		{
		case EPlayerJob::Fighter:
			//DefaultPawnClass = AWarrior::StaticClass();
			break;
		case EPlayerJob::Rogue:
			DefaultPawnClass = ARogue::StaticClass();
			break;
		case EPlayerJob::Hunter:
			DefaultPawnClass = ARanger::StaticClass();
			break;
		}
	}

	FString	PlayerName;
	FParse::Value(*Options, TEXT("PlayerName="), PlayerName);

	APlayerControllerTestGame* TestGameController =
		Cast<APlayerControllerTestGame>(result);

	if (IsValid(TestGameController))
	{
		TestGameController->SetPlayerName(PlayerName);
	}

	return result;
}