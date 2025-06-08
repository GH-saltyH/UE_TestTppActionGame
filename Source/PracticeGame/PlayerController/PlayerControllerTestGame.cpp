// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerTestGame.h"

APlayerControllerTestGame::APlayerControllerTestGame()
{
	//IMC �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputMappingContext>
		IMC(
			TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_TestGame.IMC_TestGame'")
		);
	//�����ϸ� �ּҸ� �޾ƿ´�
	if (IMC.Succeeded())
		mIMC = IMC.Object;

	//IA_Move �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAMove(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAMove.Succeeded())
		mIAMove = IAMove.Object;

	//IA_Jump �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAJump(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAJump.Succeeded())
		mIAJump = IAJump.Object;

	//IA_Attack �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAAttack(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'")
		);

	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAAttack.Succeeded())
		mIAAttack = IAAttack.Object;	
	
	//IA_CameraRotate �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACamRotate(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_CameraRotate.IA_CameraRotate'")
		);

	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IACamRotate.Succeeded())
		mIACameraRotate = IACamRotate.Object;

}

void APlayerControllerTestGame::BeginPlay()
{
	Super::BeginPlay();

	
}

void APlayerControllerTestGame::SetupInputComponent()
{
	Super::SetupInputComponent();

}