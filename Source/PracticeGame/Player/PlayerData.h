// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "PlayerData.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UPlayerData : public UObject
{
	GENERATED_BODY()

public:
	UPlayerData();
	
	//테이블을 들고 있는다
	TObjectPtr<UDataTable>	mTable;
	TObjectPtr<UDataTable>	mExpTable;

public:
	FPlayerTableInfo* FindPlayerInfo(const FName& Name)	const;
	FLevelExpRow* FindExpInfo(const FName& Name)	const;
};

