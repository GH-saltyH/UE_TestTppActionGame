// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameMode.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/Rogue/Rogue.h"
#include "../Player/Ranger/Ranger.h"
#include "../PlayerController/PlayerControllerTestGame.h"

ATestGameMode::ATestGameMode()
{

	PlayerControllerClass = APlayerControllerTestGame::StaticClass();
	DefaultPawnClass = ARogue::StaticClass();
}

APlayerController* ATestGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* Result = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	//int32 로 된 문자를 Job= 이후에서 파싱하여  Job 변수에 저장
	FParse::Value(*Options, TEXT("Job="), mSelectedJob);
	//FString 타입의 문자를 PlayerName= 이후에서 파싱하여 mInputPlayerName 변수에 저장
	FParse::Value(*Options, TEXT("PlayerName="), mPlayerName);

	if (mSelectedJob != 0)
	{
		EPlayerJob	Job = (EPlayerJob)mSelectedJob;
		switch (Job)
		{
		case EPlayerJob::Rogue:
			//기본 폰 클래스 설정 : Rogue
			DefaultPawnClass = ARogue::StaticClass();
			UE_LOG(TestGame, Warning, TEXT("mSelectedJob : Rogue -> DefaultPawnClass"));
			break;
		case EPlayerJob::Hunter:
			//기본 폰 클래스 설정 : Ranger
			DefaultPawnClass = ARanger::StaticClass();
			UE_LOG(TestGame, Warning, TEXT("mSelectedJob : Ranger -> DefaultPawnClass"));
			break;
		case EPlayerJob::Fighter:
			//기본 폰 클래스 설정 : Fighter
			DefaultPawnClass = ARogue::StaticClass();
			UE_LOG(TestGame, Warning, TEXT("mSelectedJob : Fighter -> DefaultPawnClass"));
			break;
		default:
			UE_LOG(TestGame, Warning, TEXT("mSelectedJob is Out of Range. used default : ARogue"));
			DefaultPawnClass = ARogue::StaticClass();
			break;
		}
	}

	FString	PlayerName;
	FParse::Value(*Options, TEXT("PlayerName="), PlayerName);

	APlayerControllerTestGame* TestGameController =
		Cast<APlayerControllerTestGame>(Result);

	if (IsValid(TestGameController))
	{
		TestGameController->SetPlayerName(PlayerName);
	}

	return Result;
}

	void ATestGameMode::BeginPlay()
	{
		Super::BeginPlay();
	}