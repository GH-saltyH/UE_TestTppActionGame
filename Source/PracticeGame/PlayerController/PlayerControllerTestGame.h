// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"			//UInputMappingContext 와 멤버들을 사용하려면 필요하다
#include "InputAction.h"							//UInputAction 과 멤버들을 사용하려면 필요하다
#include "PlayerControllerTestGame.generated.h"

UCLASS(Blueprintable)
class PRACTICEGAME_API APlayerControllerTestGame : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerTestGame();

public:
	//매핑컨텍스트를 들고 있는다
	TObjectPtr<UInputMappingContext> mIMC;
	//입력액션 IA_Move 를 들고 있는다
	UInputAction* mIAMove = nullptr;
	//입력액션 IA_Jump 를 들고 있는다
	UInputAction* mIAJump = nullptr;
	//입력액션 IA_Attack 을 들고있는다
	UInputAction* mIAAttack = nullptr;
	//입력액션IA_CameraRotation 을 들고있는다
	UInputAction* mIACameraRotate = nullptr;
	
protected:
	virtual void BeginPlay() override;
protected:
	//플레이어 컨트롤러가 커스텀 입력 할당을 설정할 수 있도록 해준다
	virtual void SetupInputComponent() override;
};
