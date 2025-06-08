// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerTestGame.h"

APlayerControllerTestGame::APlayerControllerTestGame()
{
	//IMC 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputMappingContext>
		IMC(
			TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_TestGame.IMC_TestGame'")
		);
	//성공하면 주소를 받아온다
	if (IMC.Succeeded())
		mIMC = IMC.Object;

	//IA_Move 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAMove(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAMove.Succeeded())
		mIAMove = IAMove.Object;

	//IA_Jump 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAJump(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAJump.Succeeded())
		mIAJump = IAJump.Object;

	//IA_Attack 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAAttack(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'")
		);

	//로드에 성공하면 주소를 받아온다
	if (IAAttack.Succeeded())
		mIAAttack = IAAttack.Object;	
	
	//IA_CameraRotate 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACamRotate(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_CameraRotate.IA_CameraRotate'")
		);

	//로드에 성공하면 주소를 받아온다
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