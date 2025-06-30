// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../UserWidgetBase.h"
#include "HPPlateWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UHPPlateWidget : public UUserWidgetBase
{
	GENERATED_BODY()

public:
	UHPPlateWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	TObjectPtr<UTextBlock> mNPCName;
	TObjectPtr<UProgressBar> mHPBar;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetNPCName(const FString& NewName);
	void SetHPBar(float HP, float MaxHP);
};
