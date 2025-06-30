// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectedNPCInfoWidget.h"


USelectedNPCInfoWidget::USelectedNPCInfoWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Constructor logic can be added here if needed
	mWidgetName = TEXT("SelectedNPCInfoWidget");
}


void USelectedNPCInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	//UI 에서 요소들의 주소를 가져온다
	//mNPCIcon = Cast<UImage>(GetWidgetFromName(TEXT("PlayerIcon")));
	mNPCName = Cast<UTextBlock>(GetWidgetFromName(TEXT("NPCName")));
	mHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPProgress")));
	mHPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPText")));
	//mMPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MPProgress")));
	//mMPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MPText")));
	//mXPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ExpProgress")));
	//mXPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ExpText")));

}

void USelectedNPCInfoWidget::SetNPCName(const FString& NewName)
{
	mNPCName->SetText(FText::FromString(NewName));
}

void USelectedNPCInfoWidget::SetHPBar(float HP, float MaxHP)
{
	mHPBar->SetPercent(HP / MaxHP);
	FString HPText = FString::Printf(TEXT("%0.0f / %0.0f"), HP, MaxHP);
	mHPText->SetText(FText::FromString(HPText));
}

//void USelectedNPCInfoWidget::SetMPBar(float MP, float MaxMP)
//{
//	mMPBar->SetPercent(MP / MaxMP);
//	FString MPText = FString::Printf(TEXT("%0.0f / %0.0f"), MP, MaxMP);
//	mMPText->SetText(FText::FromString(MPText));
//}
//
//void USelectedNPCInfoWidget::SetXPBar(int32 XP, int32 NextXP)
//{
//	mXPBar->SetPercent((float)XP / NextXP);
//	FString XPText = FString::Printf(TEXT("%d / %d"), XP, NextXP);
//	mXPText->SetText(FText::FromString(XPText));
//}


	