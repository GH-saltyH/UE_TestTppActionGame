// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UserWidgetBase.h"
#include "Character/Panel/CharacterInfoWidget.h"
#include "MainGame/NPC/SelectedNPCInfoWidget.h"
#include "../NPC/NPC.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UMainWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UMainWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	TObjectPtr<UCharacterInfoWidget>	mCharInfoWidget;
	TObjectPtr<USelectedNPCInfoWidget>	mSelectedNPCInfoWidget;

	TObjectPtr<UButton>			mButtonSkill;
	TObjectPtr<UButton>			mButtonInv;

	TObjectPtr<class UPlayerSkillPanelWidget>	mPlayerSkillPanelWidget;
	//리스트뷰 형식 인벤토리인경우
	//TObjectPtr<class UPlayerInvPanelWidget> mPlayerInvPanelWidget;
	//고정슬롯 인벤토리 인 경우
	TObjectPtr<class UPlayerInventoryFixedSlot> mPlayerInvPanelWidget;
	TObjectPtr<class UTradePanelWidget>			mTradePanelWidget;
	TObjectPtr<class UQuestWidget>				mQuestWidget;

	TObjectPtr<UUserWidget>						mDragWidget;
	FVector2D									mDragPrevPosition;

	TSubclassOf<UUserWidget> mFCTWidgetClass;
	TObjectPtr<UCanvasPanel>	mFCTArea;

public:
	//드래그 위젯의 마우스 위치를 구해놓는다, 드래그 중인 위젯의 보여지는 우선순위를 설정한다, 이전에 설정한 위젯의 우선순위는 다시 내려준다
	void SetDragWidget(UUserWidget* DragWidget);
	
protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	void ToggleCharacterInfoPanel()
	{
		if (mCharInfoWidget->IsVisible())
			mCharInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
		else
			mCharInfoWidget->SetVisibility(ESlateVisibility::Visible);
	}

	ESlateVisibility GetCharacterInfoPanelVisibility() const
	{
		return mCharInfoWidget->GetVisibility();
	}

	void CloseCharacterInfoPanel()
	{
		mCharInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

public:
	UFUNCTION()
	void ButtonSkillOnClicked();
	UFUNCTION()
	void ButtonInvOnClicked();

	void OpenTradePanel();
	void CloseTradePanel();

//FCT
public:
	void SpawnFloatingText(float Damage, ANPC* TargetActor);

};
