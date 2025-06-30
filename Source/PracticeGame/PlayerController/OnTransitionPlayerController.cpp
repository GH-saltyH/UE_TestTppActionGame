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
	// �о�� ���� Ŭ������ �̿��ؼ� ���� ��� ������ �����ϰ� ����Ʈ�� �����ش�.
	if (IsValid(mTransitionWidgetClass))
	{
		mTransitionWidget = CreateWidget<UTransitionWidget>(GetWorld(), mTransitionWidgetClass);

		if (IsValid(mTransitionWidget))
			mTransitionWidget->AddToViewport();
	}
}
