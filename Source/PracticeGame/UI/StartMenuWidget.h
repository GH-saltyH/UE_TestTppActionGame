// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UserWidgetBase.h"
#include "StartMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UStartMenuWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UStartMenuWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	TObjectPtr<UButton>		mStartButton;
	TObjectPtr<UButton>		mExitButton;

protected:
	virtual void NativeOnInitialized();

	UFUNCTION()
	void OnClickStartButton();
	UFUNCTION()
	void OnHoveredStartButton();
	UFUNCTION()
	void UnHoveredStartButton();

	UFUNCTION()
	void OnClickExitButton();
	UFUNCTION()
	void OnHoveredExitButton();
	UFUNCTION()
	void UnHoveredExitButton();

};
