// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../UI/UIInfo.h"
#include "Blueprint/UserWidget.h"
#include "QuestEntry.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UQuestEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UQuestEntry(const FObjectInitializer& ObjectInitializer);

protected:
	TObjectPtr<UTextBlock>	mTitle;
	TObjectPtr<UCheckBox>	mCheckBox;
	TObjectPtr<UCanvasPanel>	mTabCanvas;
	TObjectPtr<UObject>			mThisQuestData;
protected:
	virtual void NativeOnInitialized();

public:
	UFUNCTION(BlueprintCallable)
	void SetQuestData(UObject* DataObject);
	//마우스 플래그에 따라 개별 항목의 텍스트색상 변경
	void SetMouseState(EWidgetMouseState State);
	UFUNCTION()
	void SetShowQuest(bool bIsChecked);
};
