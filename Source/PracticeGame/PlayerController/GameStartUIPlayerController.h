// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "GameStartUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API AGameStartUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AGameStartUIPlayerController();
	
private:
	TSubclassOf<UUserWidget>		mStartWidgetClass;
	TObjectPtr<class UStartMenuWidget>	mStartWidget;
	
protected:
	virtual void BeginPlay() override;

};
