// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"			//UInputMappingContext �� ������� ����Ϸ��� �ʿ��ϴ�
#include "InputAction.h"							//UInputAction �� ������� ����Ϸ��� �ʿ��ϴ�
#include "PlayerControllerTestGame.generated.h"

UCLASS(Blueprintable)
class PRACTICEGAME_API APlayerControllerTestGame : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerTestGame();

public:
	//�������ؽ�Ʈ�� ��� �ִ´�
	TObjectPtr<UInputMappingContext> mIMC;
	//�Է¾׼� IA_Move �� ��� �ִ´�
	UInputAction* mIAMove = nullptr;
	//�Է¾׼� IA_Jump �� ��� �ִ´�
	UInputAction* mIAJump = nullptr;
	//�Է¾׼� IA_Attack �� ����ִ´�
	UInputAction* mIAAttack = nullptr;
	//�Է¾׼�IA_CameraRotation �� ����ִ´�
	UInputAction* mIACameraRotate = nullptr;
	
protected:
	virtual void BeginPlay() override;
protected:
	//�÷��̾� ��Ʈ�ѷ��� Ŀ���� �Է� �Ҵ��� ������ �� �ֵ��� ���ش�
	virtual void SetupInputComponent() override;
};
