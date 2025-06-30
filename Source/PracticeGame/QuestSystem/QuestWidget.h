// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UI/UserWidgetBase.h"
#include "QuestWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UQuestWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UQuestWidget(const FObjectInitializer& ObjectInitializer);

protected:
	//각 슬롯
	UPROPERTY(EditAnywhere)
	TObjectPtr<UButton>			mButtonDragBar;
	TObjectPtr<UButton>			mButtonExit;
	TObjectPtr<UTreeView>		 mQuestTree;
	TObjectPtr<UMultiLineEditableTextBox>		mObjective;
	TObjectPtr<UMultiLineEditableTextBox>		mDescription;
	TObjectPtr<UMultiLineEditableTextBox>		mTextRewards;


	TObjectPtr<UButton>			mButtonAccept;
	TObjectPtr<UButton>			mButtonDecline;
	TObjectPtr<UButton>			mButtonComplete;

	TObjectPtr<UTextBlock>		mTextAccept;
	TObjectPtr<UTextBlock>		mTextDecline;
	TObjectPtr<UTextBlock>		mTextComplete;

	TObjectPtr<class UQuesstEntry> mQuestTreeClass;
	class UQuestData* mSelected = nullptr;
	class UQuestData* mHovered = nullptr;

protected:
	virtual void NativeOnInitialized();
	virtual void NativeTick(const FGeometry& MyGeometry,
		float InDeltaTime);
public:
	void InitQuest();
public:
	UFUNCTION()
	void ButtonCloseOnClick();

	UFUNCTION()
	void TitleMouseDown();

	UFUNCTION()
	void TitleMouseUp();
	
	//OnGetChildren 바인딩
	//퀘스트 차일드 델리게이트 함수 만들기 
	UFUNCTION()
	void GetQuestChild(UObject* Data, TArray<UObject*>& ChildArray);
	
	//OnGetChildren 바인딩
	//UFUNCTION()
	//void OnGetQuestChildren(UObject* Item, TArray<UObject*>& OutChildren);

	UFUNCTION()
	void TreeItemHovered(UObject* Obj, bool IsHovered);

	UFUNCTION()
	void TreeItemSelect(UObject* Obj);

	UFUNCTION()
	UWidget* OnGenerateQuestRow(UObject* Item);

	//전달받은 FName 타입의 QuestID 를 가지고 원본 데이터의 해당 퀘스트 데이터행을 찾아서 반환한다
	const FQuestInfo* GetValueFromQuestDataByQuestID(FName ToFindQuestID);

	UFUNCTION()
	void ButtonAcceptOnClicked();
	UFUNCTION()
	void ButtonDeclineOnClicked();
	UFUNCTION()
	void ButtonCompleteOnClicked();

	void UpdateDescriptionPanel(UQuestData* Obj);
	UFUNCTION()
	void OnTaskUpdate(EQuestTaskType TaskType, AActor* Target);
};
