// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSkillPanelWidget.h"
#include "../SkillListData.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../../GameMode/TestGameMode.h"

UPlayerSkillPanelWidget::UPlayerSkillPanelWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	mWidgetName = TEXT("PlayerSkillPanelWidget");
}

void UPlayerSkillPanelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mSkillListView = Cast<UListView>(GetWidgetFromName(TEXT("SkillListView")));
	mButtonDragBar = Cast<UButton>(GetWidgetFromName(TEXT("ButtonDrag")));
	//mButtonExit = Cast<UButton>(GetWidgetFromName(TEXT("ButtonExit")));

	//닫기 버튼 델리게이트 함수 바인딩
	//mButtonExit->OnClicked.AddDynamic(this, &UPlayerInventoryFixedSlot::ButtonCloseOnClick);

	if(mButtonDragBar)
	{
		mButtonDragBar->OnPressed.AddDynamic(this, &UPlayerSkillPanelWidget::TitleMouseDown);
		mButtonDragBar->OnReleased.AddDynamic(this, &UPlayerSkillPanelWidget::TitleMouseUp);

		UE_LOG(TestGame, Warning, TEXT("SkillPanel : mButtonDragBar function delegate bound"));
	}
}

void UPlayerSkillPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// 1. SkillEntryList용 데이터 준비
	TArray<USkillListData*> SkillDataArray;

	UDataTable* DTSkillInfo = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/Player/Skills/DataTable/DT_PlayerSkillInfo.DT_PlayerSkillInfo'"));
	if (DTSkillInfo)
	{
		UE_LOG(TestGame, Warning, TEXT("Skill Data loaded"));
		TArray<FPlayerSkillData*> AllRows;
		DTSkillInfo->GetAllRows(TEXT("SkillRowLookup"), AllRows);

		APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayerPawn());
		if (Player)
		{
			UWorld* World = GetWorld();
			if (!World)
			{
				UE_LOG(TestGame, Warning, TEXT("Unable to get World!"));
				return;
			}

			UE_LOG(TestGame, Warning, TEXT("SkillPanel : A Player Character Cast Succeed"));
			ATestGameMode* MyGameMode = Cast<ATestGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

			if (MyGameMode)
			{
				// GameMode에 접근 가능
				EPlayerJob Job = (EPlayerJob)MyGameMode->GetSelectedPlayerJob();

				for (FPlayerSkillData* Row : AllRows)
				{
					if (Row->Job == Job)
					{
						UE_LOG(TestGame, Warning, TEXT("Found Matching Row --> Add Skill"));
						USkillListData* SkillObj = NewObject<USkillListData>();
						SkillObj->SetSkillData(*Row);  // 또는 직접 할당
						mSkillListView->AddItem(SkillObj);
					}
				}
			}
		}
	}
}


void UPlayerSkillPanelWidget::TitleMouseDown()
{

	UE_LOG(TestGame, Warning, TEXT("SkillPanel : Title Mouse Down"));

	//메인위젯에 드래그 위젯으로 이 위젯을 전달한다.  
	//메인위젯 단위에서 드래그 행위와 관련된 계산을 하는데 유용하게 될 것이다
	UMainWidget* MainWidget = Cast<UMainWidget>(mParentWidget);

	if (MainWidget)
	{
		MainWidget->SetDragWidget(this);
		MainWidget->SetFocusWidget(this);
	}
}

void UPlayerSkillPanelWidget::TitleMouseUp()
{
	UE_LOG(TestGame, Warning, TEXT("SkillPanel : Title Mouse Up"));

	//마우스가 떼지면 드래그 위젯 해제 
	UMainWidget* MainWidget = Cast<UMainWidget>(mParentWidget);

	if (MainWidget)
	{
		MainWidget->SetDragWidget(nullptr);
	}

}