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

	// 읽어온 위젯 클래스를 이용해서 실제 사용 위젯을 생성하고 뷰포트에 보여준다.
	if (IsValid(mStartWidgetClass))
	{
		mStartWidget = CreateWidget<UStartMenuWidget>(GetWorld(), mStartWidgetClass);

		if (IsValid(mStartWidget))
			mStartWidget->AddToViewport();

	}
}
