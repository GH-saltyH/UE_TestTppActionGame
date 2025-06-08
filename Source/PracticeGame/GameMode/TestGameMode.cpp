// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameMode.h"
#include "../Player/PlayerCharacter.h"
#include "../PlayerController/PlayerControllerTestGame.h"

ATestGameMode::ATestGameMode()
{
	//�⺻ �� Ŭ���� ����
	DefaultPawnClass = APlayerCharacter::StaticClass();

	//�÷��̾� ��Ʈ�ѷ� Ŭ���� ����
	PlayerControllerClass = APlayerControllerTestGame::StaticClass();
}
