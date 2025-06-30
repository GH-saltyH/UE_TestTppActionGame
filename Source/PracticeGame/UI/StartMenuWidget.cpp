// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuWidget.h"


UStartMenuWidget::UStartMenuWidget(const FObjectInitializer& ObjectInitializer) :
	UUserWidgetBase(ObjectInitializer)
{
	mWidgetName = TEXT("StartMenuWidget");
}

void UStartMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnInitialized"));

	//���� �̸��� ������ �ҷ��� �� �ִ�
	mStartButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonStart")));
	mExitButton = Cast<UButton>(GetWidgetFromName(TEXT("ButtonExit")));

	//������ �̺�Ʈ �Լ��� ���ε� �Ѵ�
	mStartButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnClickStartButton);
	mStartButton->OnHovered.AddDynamic(this, &UStartMenuWidget::OnHoveredStartButton);
	mStartButton->OnUnhovered.AddDynamic(this, &UStartMenuWidget::UnHoveredStartButton);

	mExitButton->OnClicked.AddDynamic(this, &UStartMenuWidget::OnClickExitButton);
	mExitButton->OnHovered.AddDynamic(this, &UStartMenuWidget::OnHoveredStartButton);
	mExitButton->OnUnhovered.AddDynamic(this, &UStartMenuWidget::UnHoveredStartButton);

}

void UStartMenuWidget::OnClickStartButton()
{
	//CharacterSelect �̸����� �� ������ �����Ѵ�
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("CharacterSelect"));
}

void UStartMenuWidget::OnHoveredStartButton()
{
	PlayWidgetAnimation("ButtonStartScale");
}

void UStartMenuWidget::UnHoveredStartButton()
{
	PlayWidgetAnimation("ButtonStartScale", 0, 1, false);
}

void UStartMenuWidget::OnClickExitButton()
{
	//������ �����Ѵ�
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);

}

void UStartMenuWidget::OnHoveredExitButton()
{
	PlayWidgetAnimation("ButtonExitScale");
}

void UStartMenuWidget::UnHoveredExitButton()
{
	PlayWidgetAnimation("ButtonExitScale", 0, 1, false);
}
