// Fill out your copyright notice in the Description page of Project Settings.


#include "UIGameMode.h"
#include "../PlayerController/GameStartUIPlayerController.h"

AUIGameMode::AUIGameMode()
{
	//����Ʈ��Ŭ������ ���� ó��
	DefaultPawnClass = nullptr;
	//�� ���Ӹ���� �÷��̾� ��Ʈ�ѷ� ����
	//PlayerControllerClass = AGameStartUIPlayerController::StaticClass();
}

void AUIGameMode::BeginPlay()
{
	Super::BeginPlay();
}