// Fill out your copyright notice in the Description page of Project Settings.


#include "OnTransitionPlayerController.h"

AOnTransitionPlayerController::AOnTransitionPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> TransitionWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_TransitionLevel.UI_TransitionLevel_C'"));

	if(TransitionWidgetClass.Succeeded())
		mTransitionWidgetClass = TransitionWidgetClass.Class;


}

void AOnTransitionPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
	
	// Set the player controller to be hidden during the transition
	// 읽어온 위젯 클래스를 이용해서 실제 사용 위젯을 생성하고 뷰포트에 보여준다.
	if (IsValid(mTransitionWidgetClass))
	{
		mTransitionWidget = CreateWidget<UTransitionWidget>(GetWorld(), mTransitionWidgetClass);

		if (IsValid(mTransitionWidget))
			mTransitionWidget->AddToViewport();
	}
}
