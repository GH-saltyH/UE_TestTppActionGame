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

	//���� �̸��� ������ �ҷ��� �� �ִ�
	mStartButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonSelectCharacter")));
	mExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonMainMenu")));
	mInfoWidget = Cast<USelectInfoWidget>(GetWidgetFromName(TEXT("UI_SelectCharInfo")));

	//�̸� �Է�â �޾ƿ��� UEditableText -> ���������ؽ�Ʈ  ,  UEditableTextBox -> �ؽ�Ʈ�ڽ�
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

	//�ɼ��� �����.  �� �ɼ��� ���鹮�ڷ� �����Ϸ��Ѵ�
	// ���� �ɼ�  Job, MapName, PlayerName
	FString Option = FString::Printf(TEXT("Job=%d MapName=DesertMountains MapPath=/Game/Brushify/Maps/DesertMountains/DesertMountains PlayerName=%s "), (int32)mSelectedJob, *PlayerName);

	//�ɼ��� �����ϸ鼭 Ʈ������ ������ �̵��Ѵ�. ���⼭�� �񵿱� �ε带 ó���ϰ��� �Ϸ�Ǹ� ���� ������ �̵��ϰ� ���� ���̴�.
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("LevelTransition"), true, Option);
}

void UCharacterSelectWidget::ExitButtonOnClick()
{

}