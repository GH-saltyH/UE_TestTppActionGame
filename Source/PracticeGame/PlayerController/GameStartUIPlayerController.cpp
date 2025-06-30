// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStartUIPlayerController.h"
#include "../UI/StartMenuWidget.h"

AGameStartUIPlayerController::AGameStartUIPlayerController()
{

	static ConstructorHelpers::FClassFinder<UUserWidget>
		StartWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_StartMenu.UI_StartMenu_C'"));

	if (StartWidgetClass.Succeeded())
		mStartWidgetClass = StartWidgetClass.Class;

}

void AGameStartUIPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;

	SetInputMode(InputMode);

	// �о�� ���� Ŭ������ �̿��ؼ� ���� ��� ������ �����ϰ� ����Ʈ�� �����ش�.
	if (IsValid(mStartWidgetClass))
	{
		mStartWidget = CreateWidget<UStartMenuWidget>(GetWorld(), mStartWidgetClass);

		if (IsValid(mStartWidget))
			mStartWidget->AddToViewport();

	}
}
