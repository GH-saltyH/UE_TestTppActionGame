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

	//�ݱ� ��ư ��������Ʈ �Լ� ���ε�
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
	
	// 1. SkillEntryList�� ������ �غ�
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
				// GameMode�� ���� ����
				EPlayerJob Job = (EPlayerJob)MyGameMode->GetSelectedPlayerJob();

				for (FPlayerSkillData* Row : AllRows)
				{
					if (Row->Job == Job)
					{
						UE_LOG(TestGame, Warning, TEXT("Found Matching Row --> Add Skill"));
						USkillListData* SkillObj = NewObject<USkillListData>();
						SkillObj->SetSkillData(*Row);  // �Ǵ� ���� �Ҵ�
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

	//���������� �巡�� �������� �� ������ �����Ѵ�.  
	//�������� �������� �巡�� ������ ���õ� ����� �ϴµ� �����ϰ� �� ���̴�
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

	//���콺�� ������ �巡�� ���� ���� 
	UMainWidget* MainWidget = Cast<UMainWidget>(mParentWidget);

	if (MainWidget)
	{
		MainWidget->SetDragWidget(nullptr);
	}

}