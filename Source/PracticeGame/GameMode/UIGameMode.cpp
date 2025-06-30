// Fill out your copyright notice in the Description page of Project Settings.


#include "UIGameMode.h"
#include "../PlayerController/GameStartUIPlayerController.h"

AUIGameMode::AUIGameMode()
{
	//디폴트폰클래스를 없음 처리
	DefaultPawnClass = nullptr;
	//이 게임모드의 플레이어 컨트롤러 설정
	//PlayerControllerClass = AGameStartUIPlayerController::StaticClass();
}

void AUIGameMode::BeginPlay()
{
	Super::BeginPlay();
}