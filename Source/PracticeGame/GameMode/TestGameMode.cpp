// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGameMode.h"
#include "../Player/PlayerCharacter.h"
#include "../PlayerController/PlayerControllerTestGame.h"

ATestGameMode::ATestGameMode()
{
	//기본 폰 클래스 설정
	DefaultPawnClass = APlayerCharacter::StaticClass();

	//플레이어 컨트롤러 클래스 설정
	PlayerControllerClass = APlayerControllerTestGame::StaticClass();
}
