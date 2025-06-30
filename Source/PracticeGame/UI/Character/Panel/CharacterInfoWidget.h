// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../UserWidgetBase.h"
#include "../../../Interface/CharacterPublicInterface.h"
#include "CharacterInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UCharacterInfoWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UCharacterInfoWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	//TObjectPtr<UCharacterInfoWidget> mCharInfoWidget;
	TObjectPtr<UTextBlock>	mPlayerNameInfo;
	TObjectPtr<UTextBlock>	mPlayerLevelInfo;
	TObjectPtr<UTextBlock>	mPlayerExpInfo;
	TObjectPtr<UTextBlock>	mPlayerHPInfo;
	TObjectPtr<UTextBlock>	mPlayerMPInfo;
	TObjectPtr<UTextBlock>	mPlayerAttackInfo;
	TObjectPtr<UTextBlock>	mPlayerAttackRangeInfo;
	TObjectPtr<UTextBlock>	mPlayerDefenseInfo;
	TObjectPtr<UTextBlock>	mPlayerMoveSpeedInfo;

	TObjectPtr<UButton>		mCloseButton;

	TObjectPtr<UButton>		mButtonDragBar;
protected:
	virtual void NativeOnInitialized() override;

public:
	void SetPlayerNameInfo(const FString& NewName);
	void SetPlayerLevelInfo(const FString& NewLevel);
	void SetPlayerExpInfo(const FString& NewExp);
	void SetPlayerHPInfo(const FString& NewHP);
	void SetPlayerMPInfo(const FString& NewMP);
	void SetPlayerAttackInfo(const FString& NewAttack);
	void SetPlayerAttackRangeInfo(const FString& NewAttackRange);
	void SetPlayerDefenseInfo(const FString& NewDefense);
	void SetPlayerMoveSpeedInfo(const FString& NewSpeed);

	void UpdatePlayerInfo(ICharacterPublicInterface* Info);

public:
	UFUNCTION()
	void OnClickedCloseButton();
	UFUNCTION()
	void TitleMouseDown();
	UFUNCTION()
	void TitleMouseUp();
};
