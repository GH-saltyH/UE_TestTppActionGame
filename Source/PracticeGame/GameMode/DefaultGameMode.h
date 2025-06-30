// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Player/Rogue/Rogue.h"
#include "GameFramework/GameModeBase.h"
#include "DefaultGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API ADefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ADefaultGameMode();
	
public:
	virtual APlayerController* Login(UPlayer* NewPlayer,
		ENetRole InRemoteRole, const FString& Portal,
		const FString& Options, const FUniqueNetIdRepl& UniqueId,
		FString& ErrorMessage);
};
