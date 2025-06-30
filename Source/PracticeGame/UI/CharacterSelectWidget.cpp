// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectWidget.h"
#include "../Interface/CharacterPublicInterface.h"

UCharacterSelectWidget::UCharacterSelectWidget(const FObjectInitializer& ObjectInitializer) :
	UUserWidgetBase(ObjectInitializer)
{
	mWidgetName = TEXT("CharacterSelectWidget");
}

void UCharacterSelectWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnInitialized"));

	//만든 이름을 가지고 불러올 수 있다
	mStartButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonSelectCharacter")));
	mExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonMainMenu")));
	mInfoWidget = Cast<USelectInfoWidget>(GetWidgetFromName(TEXT("UI_SelectCharInfo")));

	//이름 입력창 받아오기 UEditableText -> 편집가능텍스트  ,  UEditableTextBox -> 텍스트박스
	//mInputName = Cast<UEditableText>(GetWidgetFromName(TEXT("InputName")));
	mInputName = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("InputName")));
	//UE_LOG(TestGame, Warning, TEXT("InputName Set to %s"), *mInputName->GetName());

	mStartButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::StartButtonOnClick);
	mExitButton->OnClicked.AddDynamic(this, &UCharacterSelectWidget::ExitButtonOnClick);
}

void UCharacterSelectWidget::EnableStartButton(bool IsEnable)
{
	mStartButton->SetIsEnabled(IsEnable);
}
void UCharacterSelectWidget::EnableInfoWidget(bool IsEnable)
{
	if (IsEnable)
	{
		mInfoWidget->SetVisibility(ESlateVisibility::Visible);
		mInputName->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		mInfoWidget->SetVisibility(ESlateVisibility::Collapsed);
		mInputName->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UCharacterSelectWidget::SetCharacterImage(UMaterialInterface* Mtrl)
{
	mInfoWidget->SetCharacterImage(Mtrl);
}

void UCharacterSelectWidget::SetCharacterInfo(class ICharacterPublicInterface* Info)
{
	mInfoWidget->SetCharacterInfo(Info);
}

void UCharacterSelectWidget::StartButtonOnClick()
{
	if (!mInputName)
	{
		UE_LOG(TestGame, Warning, TEXT("mInputName is Null!"));
		return;
	}

	FString PlayerName = mInputName->GetText().ToString();	

	if(PlayerName.IsEmpty())
	{
		UE_LOG(TestGame, Warning, TEXT("Player name is empty!"));
		return;
	}	

	//옵션을 만든다.  각 옵션은 공백문자로 구분하려한다
	// 만들 옵션  Job, MapName, PlayerName
	FString Option = FString::Printf(TEXT("Job=%d MapName=DesertMountains MapPath=/Game/Brushify/Maps/DesertMountains/DesertMountains PlayerName=%s "), (int32)mSelectedJob, *PlayerName);

	//옵션을 전달하면서 트랜지션 레벨로 이동한다. 여기서는 비동기 로드를 처리하고나서 완료되면 실제 레벨로 이동하게 해줄 것이다.
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LevelTransition"), true, Option);
}

void UCharacterSelectWidget::ExitButtonOnClick()
{

}