// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterInfoWidget.h"
#include "../../../Player/PlayerCharacter.h"
#include "../../MainWidget.h"
#include "../../WidgetManager.h"

UCharacterInfoWidget::UCharacterInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	mWidgetName = TEXT("CharacterInfoWidget");
	UE_LOG(TestGame, Warning, TEXT("PlayerInfoPanel : Constructor Called"));
}

void UCharacterInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UE_LOG(TestGame, Warning, TEXT("PlayerInfoPanel : NativeOninitialized Called"));

	//mCharInfoWidget = Cast<UCharacterInfoWidget>(GetWidgetFromName(TEXT("UI_CharacterInfoWidget")));
	//UI 에서 요소들의 주소를 가져온다
	mPlayerNameInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerNameInfo")));
	mPlayerLevelInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerLevelInfo")));
	mPlayerExpInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerExpInfo")));
	mPlayerHPInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerHPInfo")));
	mPlayerMPInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerMPInfo")));
	mPlayerAttackInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerAttackInfo")));
	mPlayerAttackRangeInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerAttackRangeInfo")));
	mPlayerDefenseInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerDefenseInfo")));
	mPlayerMoveSpeedInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerMoveSpeedInfo")));

	mCloseButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonClosePanel")));
	mCloseButton->OnClicked.AddDynamic(this, &UCharacterInfoWidget::OnClickedCloseButton);

	mButtonDragBar = Cast<UButton>(GetWidgetFromName(TEXT("ButtonDrag")));

	if(mButtonDragBar)
	{ 
		mButtonDragBar->OnPressed.AddDynamic(this, &UCharacterInfoWidget::TitleMouseDown);
		mButtonDragBar->OnReleased.AddDynamic(this, &UCharacterInfoWidget::TitleMouseUp);
	}
}

void UCharacterInfoWidget::SetPlayerLevelInfo(const FString& NewName)
{
	mPlayerNameInfo->SetText(FText::FromString(NewName));
}

void UCharacterInfoWidget::SetPlayerExpInfo(const FString& NewExp)
{
	mPlayerExpInfo->SetText(FText::FromString(NewExp));
}

void UCharacterInfoWidget::SetPlayerHPInfo(const FString& NewHP)
{
	mPlayerHPInfo->SetText(FText::FromString(NewHP));
}

void UCharacterInfoWidget::SetPlayerMPInfo(const FString& NewMP)
{
	mPlayerMPInfo->SetText(FText::FromString(NewMP));
}

void UCharacterInfoWidget::SetPlayerAttackInfo(const FString& NewAttack)
{
	mPlayerAttackInfo->SetText(FText::FromString(NewAttack));
}

void UCharacterInfoWidget::SetPlayerAttackRangeInfo(const FString& NewAttackRange)
{
	mPlayerAttackRangeInfo->SetText(FText::FromString(NewAttackRange));
}

void UCharacterInfoWidget::SetPlayerDefenseInfo(const FString& NewDefense)
{
	mPlayerDefenseInfo->SetText(FText::FromString(NewDefense));
}

void UCharacterInfoWidget::SetPlayerMoveSpeedInfo(const FString& NewSpeed)
{
	mPlayerMoveSpeedInfo->SetText(FText::FromString(NewSpeed));
}

void UCharacterInfoWidget::UpdatePlayerInfo(ICharacterPublicInterface* Info)
{
	
	if(!Info)
	{
		UE_LOG(TestGame, Warning, TEXT("UpdateInfo Panel has been Terminated : No Info"));
		return;
	}
	UE_LOG(TestGame, Warning, TEXT("PlayerInfoPanel : UpdatePlayerInfo Called"));
	FString PlayerName = Info->GetName();
	FString PlayerLevel = FString::Printf(TEXT("Level : %d"), Info->GetLevel());
	FString PlayerExp = FString::Printf(TEXT("Total Exp. : %d"), Info->GetExp());
	FString PlayerHP = FString::Printf(TEXT("Maximum HP : %0.0f"), Info->GetMaxHP());
	FString PlayerMP = FString::Printf(TEXT("Maximum MP : %0.0f"), Info->GetMaxMP());
	FString PlayerAttack = FString::Printf(TEXT("Attack Power : %0.0f"), Info->GetAttack());
	FString PlayerAttackRange = FString::Printf(TEXT("Attack Range : %0.2fm"), Info->GetAttackDistance()/100);
	FString PlayerDefense = FString::Printf(TEXT("Defense : %0.0f"), Info->GetDefense());
	FString PlayerMoveSpeed = FString::Printf(TEXT("MoveSpeed : %0.2fm/S"), Info->GetMoveSpeed()/100);

	mPlayerNameInfo->SetText(FText::FromString(PlayerName));
	mPlayerLevelInfo->SetText(FText::FromString(PlayerLevel));;
	mPlayerExpInfo->SetText(FText::FromString(PlayerExp));;
	mPlayerHPInfo->SetText(FText::FromString(PlayerHP));;
	mPlayerMPInfo->SetText(FText::FromString(PlayerMP));;
	mPlayerAttackInfo->SetText(FText::FromString(PlayerAttack));;
	mPlayerAttackRangeInfo->SetText(FText::FromString(PlayerAttackRange));;
	mPlayerDefenseInfo->SetText(FText::FromString(PlayerDefense));;
	mPlayerMoveSpeedInfo->SetText(FText::FromString(PlayerMoveSpeed));;
}

void UCharacterInfoWidget::OnClickedCloseButton()
{
	UE_LOG(TestGame, Warning, TEXT("PlayerInfoPanel : Close Button Released"));
	UMainWidget* MainWidget = CWidgetManager::GetInst()->FindWidget<UMainWidget>(TEXT("MainWidget"));
	if (MainWidget)
	{
		MainWidget->CloseCharacterInfoPanel();
	}
}

void UCharacterInfoWidget::TitleMouseDown()
{

	UE_LOG(TestGame, Warning, TEXT("CharInfo : Title Mouse Down"));

	//메인위젯에 드래그 위젯으로 이 위젯을 전달한다.  
	//메인위젯 단위에서 드래그 행위와 관련된 계산을 하는데 유용하게 될 것이다
	UMainWidget* MainWidget = Cast<UMainWidget>(mParentWidget);

	if (MainWidget)
	{
		MainWidget->SetDragWidget(this);
		MainWidget->SetFocusWidget(this);
	}
}

void UCharacterInfoWidget::TitleMouseUp()
{
	UE_LOG(TestGame, Warning, TEXT("CharInfo : Title Mouse Up"));

	//마우스가 떼지면 드래그 위젯 해제 
	UMainWidget* MainWidget = Cast<UMainWidget>(mParentWidget);

	if (MainWidget)
	{
		MainWidget->SetDragWidget(nullptr);
	}
}


