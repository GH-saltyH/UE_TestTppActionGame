// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Engine/GameInstance.h"
#include "../Data/Item/ItemDataCacheManager.h"
#include "TestGameGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UTestGameGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTestGameGameInstance();

public:
	virtual void Init();
	virtual void Shutdown();
	
};
