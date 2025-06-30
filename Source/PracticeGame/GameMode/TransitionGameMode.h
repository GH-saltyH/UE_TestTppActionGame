// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "TransitionGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API ATransitionGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATransitionGameMode();

protected:
	int32	mSelectedJob;
	FString mNextMapName;
	FString mInputPlayerName;
	
public:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage);
	virtual void InitGameState();

	virtual void StartPlay();

	//로그인 전에 콜
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage);
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage);
	//로그인 이후 콜
	virtual void PostLogin(APlayerController* NewPlayer);

	virtual void Logout(AController* Exiting);

protected:
	virtual void BeginPlay();

public:

	void LoadingAsyncComplete(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result);
};
