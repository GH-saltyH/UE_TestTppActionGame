// Fill out your copyright notice in the Description page of Project Settings.


#include "HPPlateWidget.h"
#include "../FloatingCombatTextWidget.h"

UHPPlateWidget::UHPPlateWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	mWidgetName = TEXT("HPPlateWidget");

}

void UHPPlateWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	//UI 에서 요소들의 주소를 가져온다
	mNPCName = Cast<UTextBlock>(GetWidgetFromName(TEXT("NPCName")));
	mHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPProgress")));

	//델리게이트 바인딩
}

void UHPPlateWidget::SetNPCName(const FString& NewName)
{
	UE_LOG(TestGame, Warning, TEXT("NPC : SetNPCName"));
	mNPCName->SetText(FText::FromString(NewName));
}

void UHPPlateWidget::SetHPBar(float HP, float MaxHP)
{
	UE_LOG(TestGame, Warning, TEXT("NPC : SetHPBar"));
	mHPBar->SetPercent(HP / MaxHP);
}
