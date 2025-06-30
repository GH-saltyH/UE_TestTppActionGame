// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../UserWidgetBase.h"
#include "PlayerSkillPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UPlayerSkillPanelWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UPlayerSkillPanelWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	TObjectPtr<UListView>		mSkillListView;
	TObjectPtr<UButton>			mButtonDragBar;

protected:
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();

public:
	//�̵������ִ� ��� �Լ�
	UFUNCTION()
	void TitleMouseDown();
	UFUNCTION()
	void TitleMouseUp();

};
