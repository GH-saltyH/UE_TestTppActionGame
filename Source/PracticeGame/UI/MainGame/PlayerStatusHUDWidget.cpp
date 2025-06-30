// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatusHUDWidget.h"

UPlayerStatusHUDWidget::UPlayerStatusHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Constructor logic can be added here if needed
	mWidgetName = TEXT("PlayerStatusHUDWidget");
}


void UPlayerStatusHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	//UI 에서 요소들의 주소를 가져온다
	mPlayerIcon = Cast<UImage>(GetWidgetFromName(TEXT("PlayerIcon")));
	mPlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("PlayerName")));
	mHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPProgress")));
	mHPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("HPText")));
	mMPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("MPProgress")));
	mMPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("MPText")));
	mXPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ExpProgress")));
	mXPText = Cast<UTextBlock>(GetWidgetFromName(TEXT("ExpText")));

}

void UPlayerStatusHUDWidget::SetPlayerName(const FString& NewName)
{
	mPlayerName->SetText(FText::FromString(NewName));	
}

void UPlayerStatusHUDWidget::SetHPBar(float HP, float MaxHP)
{
	mHPBar->SetPercent(HP / MaxHP);
	FString HPText = FString::Printf(TEXT("%0.0f / %0.0f"), HP, MaxHP);
	mHPText->SetText(FText::FromString(HPText));
}

void UPlayerStatusHUDWidget::SetMPBar(float MP, float MaxMP)
{
	mMPBar->SetPercent(MP / MaxMP);
	FString MPText = FString::Printf(TEXT("%0.0f / %0.0f"), MP, MaxMP);
	mMPText->SetText(FText::FromString(MPText));
}

void UPlayerStatusHUDWidget::SetXPBar(int32 XP, int32 NextXP)
{
	mXPBar->SetPercent((float)XP / NextXP);
	FString XPText = FString::Printf(TEXT("%d / %d"), XP, NextXP);
	mXPText->SetText(FText::FromString(XPText));
}