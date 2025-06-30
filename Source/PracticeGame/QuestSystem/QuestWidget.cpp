// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestWidget.h"
#include "../UI/MainWidget.h"
#include "../UI/WidgetManager.h"
#include "QuestData.h"
#include "QuestEntry.h"
#include "../Data/Quest/QuestDataCacheManager.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/QuestComponent.h"

UQuestWidget::UQuestWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	mWidgetName = TEXT("QuestWidget");

}

void UQuestWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//요소들 받아두기
	mButtonDragBar = Cast<UButton>(GetWidgetFromName(TEXT("ButtonDrag")));
	mButtonExit = Cast<UButton>(GetWidgetFromName(TEXT("ButtonExit")));
	mQuestTree = Cast<UTreeView>(GetWidgetFromName(TEXT("QuestTree")));
	mObjective = Cast<UMultiLineEditableTextBox>(GetWidgetFromName(TEXT("TextObjective")));
	mDescription = Cast<UMultiLineEditableTextBox>(GetWidgetFromName(TEXT("TextDescription")));
	mTextRewards = Cast<UMultiLineEditableTextBox>(GetWidgetFromName(TEXT("TextRewards")));
		
	mButtonAccept = Cast<UButton>(GetWidgetFromName(TEXT("ButtonAccept")));;
	mButtonDecline = Cast<UButton>(GetWidgetFromName(TEXT("ButtonDecline")));;
	mButtonComplete = Cast<UButton>(GetWidgetFromName(TEXT("ButtonComplete")));;

	mTextAccept = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextAccept")));;
	mTextDecline = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextDecline")));;
	mTextComplete = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextComplete")));;


	//닫기 버튼 델리게이트 함수 바인딩
	mButtonExit->OnClicked.AddDynamic(this, &UQuestWidget::ButtonCloseOnClick);
	mButtonDragBar->OnPressed.AddDynamic(this, &UQuestWidget::TitleMouseDown);
	mButtonDragBar->OnReleased.AddDynamic(this, &UQuestWidget::TitleMouseUp);

	//자식노드가 있을 때 등록해주는 함수를 호출 할 수 있게 지정한다.
	mQuestTree->SetOnGetItemChildren(this, &UQuestWidget::GetQuestChild);

	mQuestTree->OnItemIsHoveredChanged().AddUObject(this, &UQuestWidget::TreeItemHovered);
	mQuestTree->OnItemSelectionChanged().AddUObject(this, &UQuestWidget::TreeItemSelect);

	mButtonAccept->OnClicked.AddDynamic(this, &UQuestWidget::ButtonAcceptOnClicked);
	mButtonDecline->OnClicked.AddDynamic(this, &UQuestWidget::ButtonDeclineOnClicked);
	mButtonComplete->OnClicked.AddDynamic(this, &UQuestWidget::ButtonCompleteOnClicked);

	//임시로 호출 -> InitQuest()
	InitQuest();

	APlayerController* PC = GetOwningPlayer();
	//퀘스트 태스크 업데이트 델리게이트 바인딩
	if (PC)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
		if (Player)
		{
			if (UQuestComponent* QuestComponent = Player->GetQuestComponent())
			{
				QuestComponent->mOnQuestTriggered.AddUObject(this, &UQuestWidget::OnTaskUpdate);
			}
		}
	}
}

void UQuestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}

void UQuestWidget::InitQuest()
{
	//데이터 테이블에서 모든 퀘스트 데이터를 불러온다 
	 
	//이 클래스 생성자에서 CDO 로부터 데이터테이블 주소를 멤버에 얻어두었다.
	//아직 스냅샷, 세이브개념이 없으니 일단 CDO 를 가지고 전부 뿌려주는 식으로 처리해본다
	TArray<FQuestInfo*> QuestDataAll;

	const UQuestDataCacheManager* CDO = GetDefault<UQuestDataCacheManager>();
	CDO->GetQuestTable()->GetAllRows(TEXT("BuildTree"), QuestDataAll);

	//QuestID 로 TMap 만들어둔다 계층을 만들 때 이 맵으로 빠르게 구조를 만들려고 한다
	TMap<FName, UQuestData*> QuestMap;

	for (FQuestInfo* Row : QuestDataAll)
	{
		auto* Node = NewObject<UQuestData>();
		Node->mQuestID			= Row->QuestID;
		Node->mParentQuestID	= Row->ParentQuestID;
		Node->mQuestLevel		= Row->QuestLevel;
		Node->mTaskType			= Row->TaskType;
		//Node->mTitle				= Row->Title;
		//Node->mDescription		= Row->Description;
		//Node->mObjective			= Row->ObjectiveText;
		Node->mTargetOverlapVolumeClass	= Row->TargetOverlapVolumeClass;
		Node->mTargetNPCClass						= Row->TargetNPCClass;
		Node->mTargetPickupActorClass			= Row->TargetPickupActorClass;
		//string 들은 데이터 절약을 위해 만들지 않고 필요할 때 QuestID 로 탐색해서 읽어오자

		//TMap 에 추가 (ID 를 키로, 행의 전체 데이터를 데이터로)
		QuestMap.Add(Node->mQuestID, Node);
	}

	//계층구조 만들기
	TArray<UQuestData*> RootNodes;

	//각 키에 맞는 것들끼리 구조를 만들어야 한다 
	for (auto& Seek : QuestMap)
	{
		auto* Node = Seek.Value;

		//섹션인경우 루트 노드에 포함  데이터 1단위는 -> 섹션 / 메인퀘스트 / 서브퀘스트 
		if (Node->mQuestLevel == EQuestLevel::Section)
		{
			RootNodes.Add(Node);
		}
		else
		{	
			//섹션이 아니면 부모를 찾아 부모의 차일드로 추가한다
			UQuestData** Parent = QuestMap.Find(Node->mParentQuestID);
			if (Parent && *Parent)
			{
				(*Parent)->AddChild(Node);
			}
		}
	}

	//단일 아이템 추가할 때
	//mQuestTree->AddItem(Node);
	
	//통째로 만들 때 
	mQuestTree->SetListItems(RootNodes);
	mQuestTree->RequestRefresh();
}

void UQuestWidget::ButtonCloseOnClick()
{
	//창닫기 버튼
	if (IsVisible())
		SetVisibility(ESlateVisibility::Collapsed);
}

void UQuestWidget::TitleMouseDown()
{

	UE_LOG(TestGame, Warning, TEXT("Inventory Title Mouse Down"));

	//메인위젯에 드래그 위젯으로 이 위젯을 전달한다.  
	//메인위젯 단위에서 드래그 행위와 관련된 계산을 하는데 유용하게 될 것이다
	UMainWidget* MainWidget = Cast<UMainWidget>(mParentWidget);

	if (MainWidget)
	{
		MainWidget->SetDragWidget(this);
		MainWidget->SetFocusWidget(this);
	}
}

void UQuestWidget::TitleMouseUp()
{
	UE_LOG(TestGame, Warning, TEXT("Inventory Title Mouse Up"));

	//마우스가 떼지면 드래그 위젯 해제 
	UMainWidget* MainWidget = Cast<UMainWidget>(mParentWidget);

	if (MainWidget)
	{
		MainWidget->SetDragWidget(nullptr);
	}
}

//전달받은 인자에 QData 로부터  차일드를 얻어와서 추가해서 반환한다 
void UQuestWidget::GetQuestChild(UObject* Data, TArray<UObject*>& ChildArray)
{
	UQuestData* QData = Cast<UQuestData>(Data);

	for (auto& Child : QData->mChildArray)
	{
		ChildArray.Add(Child.Get());
	}
}

//자식 노드 제공자이다. 이걸 설정해야 트리뷰에 정상적으로 들어간다
//void UQuestWidget::OnGetQuestChildren(UObject* Item, TArray<UObject*>& OutChildren)
//{
//	UQuestData* Node = Cast<UQuestData>(Item);
//	if (Node)
//	{
//		for (UQuestData* Child : Node->mChildArray)
//		{
//			OutChildren.Add(Child.Get());
//		}
//	}
//}

void UQuestWidget::TreeItemHovered(UObject* Obj, bool IsHovered)
{
	UQuestData* Data = Cast<UQuestData>(Obj);

	if (Data != mSelected)
	{
		UQuestEntry* EntryWidget = mQuestTree->GetEntryWidgetFromItem<UQuestEntry>(Obj);

		if (IsValid(EntryWidget))
		{
			if (IsHovered)
			{
				EntryWidget->SetMouseState(EWidgetMouseState::Hovered);
			}
			else
			{
				EntryWidget->SetMouseState(EWidgetMouseState::None);
			}
		}
	}

	if (IsHovered)
		mHovered = Data;
	else
		mHovered = nullptr;
}

void UQuestWidget::TreeItemSelect(UObject* Obj)
{
	//기존의 객체에 할당된 플래그는 초기화한다
	if (mSelected)
	{
		mSelected->mSelect = false;

		UQuestEntry* EntryWidget =
			mQuestTree->GetEntryWidgetFromItem<UQuestEntry>(mSelected);

		if (IsValid(EntryWidget))
			EntryWidget->SetMouseState(EWidgetMouseState::None);
	}

	//새로운 선택으로 업데이트 한다
	mSelected = Cast<UQuestData>(Obj);
	
	if (mSelected)
	{
		UpdateDescriptionPanel(mSelected);

		mSelected->mSelect = true;
		
		UQuestEntry* EntryWidget =
			mQuestTree->GetEntryWidgetFromItem<UQuestEntry>(mSelected);

		if (IsValid(EntryWidget))
			EntryWidget->SetMouseState(EWidgetMouseState::Select);
	}
}

//Entry Widget Class 를 배정했다면 필요가 없다 
// 하지만 데이터 타입에 따라 동적 위젯을 변경하는 경우나
// UI 를 동적으로 생성하거나 커스터미이징된 바인딩 로직이 필요한경우 
// 아래 함수를 대신 델리게이트 바인딩해서 사용하면 커스텀 조작에 더 유리하다
UWidget* UQuestWidget::OnGenerateQuestRow(UObject* Item)
{
	auto* QuestNode = Cast<UQuestData>(Item);
	if (!QuestNode)
		return nullptr;

	//auto* EntryWidget = CreateWidget<UQuestWidget>(this, QuestEntry);

	return nullptr;
}

void UQuestWidget::ButtonAcceptOnClicked()
{
	UE_LOG(TestGame, Warning, TEXT("ButtonAccept : OnClicked!"));

	if (!mSelected)
		return;

	//아직 수락하지 않았다면 진행중으로 바꾼다
	if (mSelected->mThisProgress == EQuestProgressState::NOTACCEPTED)
	{
		mSelected->mThisProgress = EQuestProgressState::INPROGRESS;
		
		//UI 정보 및 외형 업데이트 
		UpdateDescriptionPanel(mSelected);



		//퀘스트컴포넌트에 이 퀘스트를 등록한다
		APlayerController* PC = GetOwningPlayer();
		
		if (PC)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
			if (Player)
			{
				if (UQuestComponent* QuestComponent = Player->GetQuestComponent())
				{
					QuestComponent->AcceptQuest(mSelected);
				}
			}
		}
	}
}

void UQuestWidget::ButtonDeclineOnClicked()
{
	UE_LOG(TestGame, Warning, TEXT("ButtonDecline : OnClicked!"));
	if (!mSelected)
		return;

	//진행 중이거나 달성했지만 완료하지 않은경우 NOTACCEPTED 로 바꾼다
	if (mSelected->mThisProgress == EQuestProgressState::INPROGRESS || mSelected->mThisProgress == EQuestProgressState::COMPLETED)
	{
		mSelected->mThisProgress = EQuestProgressState::NOTACCEPTED;

		//진행 상태를 초기화한다 
		mSelected->mQuestProgressCount = 0;

		//UI정보 및 외형 업데이트
		UpdateDescriptionPanel(mSelected);

		//퀘스트컴포넌트에서 이 퀘스트를 제거한다
		APlayerController* PC = GetOwningPlayer();

		if (PC)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
			if (Player)
			{
				if (UQuestComponent* QuestComponent = Player->GetQuestComponent())
				{
					QuestComponent->RemoveQuest(mSelected);
				}
			}
		}
	}
}

void UQuestWidget::ButtonCompleteOnClicked()
{
	UE_LOG(TestGame, Warning, TEXT("ButtonComplete : OnClicked!"));
	if (!mSelected)
		return;

	//COMPLETED 인 경우에  완료로 바꾼다 -> 보상을 처리하는 함수를 호출해야 할 것 같다
	if (mSelected->mThisProgress == EQuestProgressState::COMPLETED)
	{
		mSelected->mThisProgress = EQuestProgressState::REWARDCLAIMED;
		
		//UI정보 및 외형 업데이트
		UpdateDescriptionPanel(mSelected);

		//퀘스트컴포넌트에서 이 퀘스트를 제거한다
		APlayerController* PC = GetOwningPlayer();

		if (PC)
		{
			APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
			if (Player)
			{
				if (UQuestComponent* QuestComponent = Player->GetQuestComponent())
				{
					QuestComponent->RemoveQuest(mSelected);
				}
				
				//보상획득하기, CDO 로부터 퀘스트 로데이터 받아온다
				const UQuestDataCacheManager* CDO = GetDefault<UQuestDataCacheManager>();

				if (!CDO)
					return;
				
				const FQuestInfo* GetRawRow = CDO->GetRowByQuestID(mSelected->mQuestID);

				if (!GetRawRow)
					return;

				//경험치 획득
				Player->AddExp(GetRawRow->RewardExp);
				//골드 획득
				Player->AddGold(GetRawRow->RewardGold);

				//플레이어에 보상 아이템 생성 -> 개수만큼
				for (int32 i = 0; i < GetRawRow->RewardAmount ; i++)
				{
					bool bHasRewardItem = true;
					switch (GetRawRow->RewardType)
					{
					case EQuestRewardType::NONE:
						bHasRewardItem = false;
						break;
					case EQuestRewardType::GOLD:
						bHasRewardItem = false;
						break;
					case EQuestRewardType::ITEM:
						Player->GetInventoryComponent()->AddItem(GetRawRow->RewardItemCode);
						break;
					case EQuestRewardType::REPUTATION:
						bHasRewardItem = false;
						//평판시스템 미구현ㅎㅎ
						break;
					}

					if (!bHasRewardItem)
						break;
				}

				//케릭터 인포 업데이트 -> 위젯매니저에서 인포위젯 얻기
				UCharacterInfoWidget* InfoWidget = CWidgetManager::GetInst()->FindWidget<UCharacterInfoWidget>(TEXT("CharacterInfoWidget"));

				if (!InfoWidget)
					return;

				ICharacterPublicInterface* Info = Cast<ICharacterPublicInterface>(Player);

				if (!Info)
					return;

				//업데이트 함수 호출
				InfoWidget->UpdatePlayerInfo(Info);
			}
		}
	}
}

void UQuestWidget::UpdateDescriptionPanel(UQuestData* Obj)
{
	if (!mSelected)
		return;

	//CDO 에서 this 퀘스트데이터와 일치하는 행을 받아온다
	const UQuestDataCacheManager* CDO = GetDefault<UQuestDataCacheManager>();
	const FQuestInfo* RawRow = CDO->GetRowByQuestID(mSelected->mQuestID);
	if (!RawRow)
		return;

	//1. 선택한 항목으로 설명을  업데이트
	// 1a 퀘스트 목표 출력 (짧은 문장)
	if(mSelected->mThisProgress==EQuestProgressState::COMPLETED)
	{
		FString TextObjective = FString::Printf(TEXT("%s  %d/%d (Completed)"), *RawRow->ObjectiveText, mSelected->mQuestProgressCount, RawRow->RequiredCount);
		mObjective->SetText(FText::FromString(TextObjective));
	}
	else
	{
		FString TextObjective = FString::Printf(TEXT("%s  %d/%d"), *RawRow->ObjectiveText, mSelected->mQuestProgressCount, RawRow->RequiredCount);
		mObjective->SetText(FText::FromString(TextObjective));
	}
	// 1b 퀘스트 이야기 출력 (긴 문장)
	mDescription->SetText(FText::FromString(RawRow->Description));

	// 1c 보상 항목 출력
	FString RewardText;

	switch (RawRow->RewardType)
	{
	case EQuestRewardType::NONE:
		RewardText = FString::Printf(TEXT("%d Exp.\t%d Gold"), RawRow->RewardExp, RawRow->RewardGold);
		break;
	case EQuestRewardType::GOLD:
		RewardText = FString::Printf(TEXT("%d Exp.\t%d Gold"), RawRow->RewardExp, RawRow->RewardGold);
		break;
	case EQuestRewardType::ITEM:
		RewardText = FString::Printf(TEXT("%d Exp.\t%d Gold\nItemCode : [%s]"), RawRow->RewardExp, RawRow->RewardGold, *RawRow->RewardItemCode.ToString());
		break;
	case EQuestRewardType::REPUTATION:
		RewardText = FString::Printf(TEXT("%d Exp.\t%d Gold\nReputation points : %d"), RawRow->RewardExp, RawRow->RewardGold, RawRow->RewardAmount);
		break;
	}
	mTextRewards->SetText(FText::FromString(RewardText));

	//2. 버튼 외형 및 활성/비활성 설정
	FLinearColor ButtonActiveColor = FLinearColor(0.495466f, 0.495466f, 0.495466f, 1.000000f);
	FLinearColor ButtonDeactiveColor = FLinearColor(0, 0, 0, 0.47f);
	FLinearColor TextActiveColor = FLinearColor(1, 1, 1, 1);
	FLinearColor TextDeactiveColor = FLinearColor(0.495466f, 0.495466f, 0.495466f, 1.000000f);

	switch (mSelected->mThisProgress)
	{
	case EQuestProgressState::NOTACCEPTED:
		//Accept 활성화
		mButtonAccept->SetColorAndOpacity(ButtonActiveColor);
		mButtonAccept->SetVisibility(ESlateVisibility::Visible);
		mTextAccept->SetColorAndOpacity(FLinearColor(TextActiveColor));
		mTextAccept->SetVisibility(ESlateVisibility::HitTestInvisible);

		//Decline 비활성화
		mButtonDecline->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonDecline->SetVisibility(ESlateVisibility::Collapsed);
		mTextDecline->SetColorAndOpacity(TextDeactiveColor);
		mTextDecline->SetVisibility(ESlateVisibility::Collapsed);

		//Complete 비활성화
		mButtonComplete->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonComplete->SetVisibility(ESlateVisibility::Collapsed);
		mTextComplete->SetColorAndOpacity(TextDeactiveColor);
		mTextComplete->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EQuestProgressState::INPROGRESS:
		//Accept 비활성화
		mButtonAccept->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonAccept->SetVisibility(ESlateVisibility::Collapsed);
		mTextAccept->SetColorAndOpacity(FLinearColor(TextDeactiveColor));
		mTextAccept->SetVisibility(ESlateVisibility::Collapsed);

		//Decline 활성화
		mButtonDecline->SetColorAndOpacity(ButtonActiveColor);
		mButtonDecline->SetVisibility(ESlateVisibility::Visible);
		mTextDecline->SetColorAndOpacity(TextActiveColor);
		mTextDecline->SetVisibility(ESlateVisibility::HitTestInvisible);

		//Complete 비활성화
		mButtonComplete->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonComplete->SetVisibility(ESlateVisibility::Collapsed);
		mTextComplete->SetColorAndOpacity(TextDeactiveColor);
		mTextComplete->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EQuestProgressState::COMPLETED:
		//Accept 비활성화
		mButtonAccept->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonAccept->SetVisibility(ESlateVisibility::Collapsed);
		mTextAccept->SetColorAndOpacity(FLinearColor(TextDeactiveColor));
		mTextAccept->SetVisibility(ESlateVisibility::Collapsed);

		//Decline 활성화
		mButtonDecline->SetColorAndOpacity(ButtonActiveColor);
		mButtonDecline->SetVisibility(ESlateVisibility::Visible);
		mTextDecline->SetColorAndOpacity(TextActiveColor);
		mTextDecline->SetVisibility(ESlateVisibility::HitTestInvisible);

		//Complete 활성화
		mButtonComplete->SetColorAndOpacity(ButtonActiveColor);
		mButtonComplete->SetVisibility(ESlateVisibility::Visible);
		mTextComplete->SetColorAndOpacity(TextActiveColor);
		mTextComplete->SetVisibility(ESlateVisibility::HitTestInvisible);
		break;
	case EQuestProgressState::REWARDCLAIMED:
		//Accept 비활성화
		mButtonAccept->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonAccept->SetVisibility(ESlateVisibility::Collapsed);
		mTextAccept->SetColorAndOpacity(FLinearColor(TextDeactiveColor));
		mTextAccept->SetVisibility(ESlateVisibility::Collapsed);

		//Decline 비활성화
		mButtonDecline->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonDecline->SetVisibility(ESlateVisibility::Collapsed);
		mTextDecline->SetColorAndOpacity(TextDeactiveColor);
		mTextDecline->SetVisibility(ESlateVisibility::Collapsed);

		//Complete 비활성화
		mButtonComplete->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonComplete->SetVisibility(ESlateVisibility::Collapsed);
		mTextComplete->SetColorAndOpacity(TextDeactiveColor);
		mTextComplete->SetVisibility(ESlateVisibility::Collapsed);
		break;
	default:
		//Accept 비활성화
		mButtonAccept->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonAccept->SetVisibility(ESlateVisibility::Collapsed);
		mTextAccept->SetColorAndOpacity(FLinearColor(TextDeactiveColor));
		mTextAccept->SetVisibility(ESlateVisibility::Collapsed);

		//Decline 비활성화
		mButtonDecline->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonDecline->SetVisibility(ESlateVisibility::Collapsed);
		mTextDecline->SetColorAndOpacity(TextDeactiveColor);
		mTextDecline->SetVisibility(ESlateVisibility::Collapsed);

		//Complete 비활성화
		mButtonComplete->SetColorAndOpacity(ButtonDeactiveColor);
		mButtonComplete->SetVisibility(ESlateVisibility::Collapsed);
		mTextComplete->SetColorAndOpacity(TextDeactiveColor);
		mTextComplete->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}
}

void UQuestWidget::OnTaskUpdate(EQuestTaskType TaskType, AActor* Target)
{
	//퀘스트 패널의 값을 내부 값으로 업데이트하여 출력하는 함수를 호출한다
	UpdateDescriptionPanel(mSelected);
}
