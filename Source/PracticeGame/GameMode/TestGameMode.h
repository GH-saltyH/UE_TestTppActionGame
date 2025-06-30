// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "TestGameMode.generated.h"

UCLASS()
class PRACTICEGAME_API ATestGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ATestGameMode();

protected:
	int32	mSelectedJob;
	FString mPlayerName;

public:
	void SetPlayerName(const FString& NewPlayerName)
	{
		mPlayerName = NewPlayerName;
	}
	void GetPlayerName(FString& OutPlayerName) const
	{
		OutPlayerName = mPlayerName;
	}

	int32 GetSelectedPlayerJob()
	{	
		return mSelectedJob;
	}

	FString GetPlayerName() const
	{
		return mPlayerName;
	}
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage);

protected:
	virtual void BeginPlay();
};
