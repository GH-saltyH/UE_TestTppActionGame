// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCMerchant.h"
#include "../../UI/WidgetManager.h"
#include "../../UI/MainWidget.h"

ANPCMerchant::ANPCMerchant()
{
	mBody->SetCollisionProfileName(TEXT("NPC"));
}

void ANPCMerchant::OnClicked()
{
	UE_LOG(TestGame, Warning, TEXT("A NPC ->  OnClicked called!"));

	UMainWidget* MainWidget= CWidgetManager::GetInst()->FindWidget<UMainWidget>(TEXT("MainWidget"));

	if (MainWidget)
	{
		MainWidget->OpenTradePanel();
	}
}

void ANPCMerchant::OnFocusOut()
{
	UE_LOG(TestGame, Warning, TEXT("A NPC ->  OnFocusOut called!"));
	UMainWidget* MainWidget = CWidgetManager::GetInst()->FindWidget<UMainWidget>(TEXT("MainWidget"));

	if (MainWidget)
	{
		MainWidget->CloseTradePanel();
	}
}
