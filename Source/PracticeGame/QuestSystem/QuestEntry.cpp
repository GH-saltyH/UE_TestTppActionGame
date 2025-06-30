// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestEntry.h"
#include "QuestData.h"
#include "QuestWidget.h"
#include "../UI/WidgetManager.h"
#include "../Data/Quest/QuestDataCacheManager.h"

UQuestEntry::UQuestEntry(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UQuestEntry::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//요소들 받아두기
	mTitle = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestName")));
	mCheckBox = Cast<UCheckBox>(GetWidgetFromName(TEXT("Checker")));
	mTabCanvas = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("canvTab")));

	mCheckBox->OnCheckStateChanged.AddDynamic(this, &UQuestEntry::SetShowQuest);
}

void UQuestEntry::SetQuestData(UObject* DataObject)
{ 
	UQuestData* QData = Cast<UQuestData>(DataObject);
	if (!QData)
		return;

	UQuestWidget* QuestWidget = CWidgetManager::GetInst()->FindWidget<UQuestWidget>(TEXT("QuestWidget"));
	if (!QuestWidget)
		return;

	//데이터캐시매니저 CDO 에서부터 해당 ID 로데이터를 받는다
	const UQuestDataCacheManager* CDO = GetDefault<UQuestDataCacheManager>();
	const FQuestInfo* RawData = CDO->GetRowByQuestID(QData->mQuestID);
	if (!RawData)
		return;

	if (QData)
	{
		mTitle->SetText(FText::FromString(RawData->Title));
		
		//계층에 따른 탭조절, 
		// 수식 = 계층 레벨 * 미리 설정된캔버스패널의 채우기 비율 계수 
		// 여기선 0.1f씩 올리려고 한다.
		//  즉  레벨0(최상위) 인 경우  0,  다음 계층은 0.1, 그다음계층은 0.2 로 점점 탭이 늘어난다
		float TabMulti = (float)(QData->mQuestLevel);
		TabMulti *= 0.1f;

		if(UHorizontalBoxSlot* BoxSlot = Cast<UHorizontalBoxSlot>(mTabCanvas->Slot))
		{
			FSlateChildSize SizeStruct(ESlateSizeRule::Fill);
			SizeStruct.Value = TabMulti;

			BoxSlot->SetSize(SizeStruct);
		}

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(mTitle->Slot);

		if (CanvasSlot)
		{
			//하위계층 마다 텍스트 탭스페이싱 위의 방법으로 바꿈
			//CanvasSlot->SetPosition(FVector2D(QData->mTreeLevel * 20.f, 0.f));

			//이 퀘스트 항목의 텍스트박스 색상 변경하기
			if (QData->mSelect)
				mTitle->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
			else
				mTitle->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		}
	}
}

//마우스 플래그에 따라 텍스트항목 색상 변경
void UQuestEntry::SetMouseState(EWidgetMouseState State)
{
	switch (State)
	{
	case EWidgetMouseState::None:
		mTitle->SetColorAndOpacity(FLinearColor(1, 1, 1, 1));
		break;
	case EWidgetMouseState::Hovered:
		mTitle->SetColorAndOpacity(FLinearColor(0.7f, 0.7f, 0.7f, 1));
		break;
	case EWidgetMouseState::Select:
		mTitle->SetColorAndOpacity(FLinearColor(1, 0, 0, 1));
		break;
	}
}

void UQuestEntry::SetShowQuest(bool bIsChecked)
{
	UE_LOG(TestGame, Warning, TEXT("CheckBox Changed"));
}