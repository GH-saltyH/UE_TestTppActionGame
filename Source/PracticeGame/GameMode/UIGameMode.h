// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/GameModeBase.h"
#include "UIGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API AUIGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AUIGameMode();
	
protected:
	virtual void BeginPlay()	override;
	
};
