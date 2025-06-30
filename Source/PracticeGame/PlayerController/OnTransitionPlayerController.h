// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "../UI/TransitionWidget.h"
#include "OnTransitionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API AOnTransitionPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AOnTransitionPlayerController();
	
private:
	TSubclassOf<UUserWidget>		mTransitionWidgetClass;
	TObjectPtr<class UTransitionWidget>	mTransitionWidget;

protected:
	virtual void BeginPlay() override;

	
};
