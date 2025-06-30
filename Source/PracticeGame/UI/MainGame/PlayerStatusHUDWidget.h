// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../UserWidgetBase.h"
#include "PlayerStatusHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UPlayerStatusHUDWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UPlayerStatusHUDWidget(const FObjectInitializer& ObjectInitializer);	
	
protected:
	TObjectPtr<UImage> mPlayerIcon;
	TObjectPtr<UTextBlock> mPlayerName;
	TObjectPtr<UProgressBar> mHPBar;
	TObjectPtr<UTextBlock> mHPText;
	TObjectPtr<UProgressBar> mMPBar;
	TObjectPtr<UTextBlock> mMPText;
	TObjectPtr<UProgressBar> mXPBar;
	TObjectPtr<UTextBlock> mXPText;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetPlayerName(const FString& NewName);
	void SetHPBar(float HP, float MaxHP);
	void SetMPBar(float MP, float MaxMP);
	void SetXPBar(int32 XP, int32 NextXP);
};
