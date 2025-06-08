// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "../Actor/ATestProjectile.h"
#include "EnhancedInputSubsystems.h"		//향상된 입력시스템을 위해 필요
#include "EnhancedInputComponent.h"		//BindAxis BindAction 등을 위해 필요하다
#include "../PlayerController/PlayerControllerTestGame.h"
#include "PlayerCharacter.generated.h"

//플레이어캐릭터 관련 로그설정
DECLARE_LOG_CATEGORY_EXTERN(PlayerLog, Log, All);

UCLASS()	//Blueprintable = 블루 프린트에서 확장 가능
class PRACTICEGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

private:
	//디버그 메시지 출력 토글
	UPROPERTY(EditAnywhere)
	bool bDebugEnabled = false;
	//스프링 암을 가지고 있을 멤버를 선언한다
	//에디터에서 수정이 가능하다
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> mSpringArm;

	//카메라 컴포넌트를 가지고 있을  멤버를 선언한다
	//게임 실행 중에 볼 수 있다
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent>	mCamera;

	//애님인스턴스를 받아둘 포인터
	TObjectPtr<class UAnimInstancePlayer>	mAnimInst;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AATestProjectile> mProjectileClass;

	//케릭터 상태 관리를 위한 구조체이다  비트플래그로  비트연산자를 지원한다
	//8비트이기 때문에 최대 8개의 상태를 저장할 수 있다
	FCharacterState	mCharacterState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//입력 액션에 의해 바인딩할 함수는
	// 반드시 void 반환타입
	// const FInputActionValue& Value 인자 타입을 설정해야 한다

	//1. 이동입력::Triggered을 처리하는 바인딩 함수
	void OnIAMoveTriggered(const FInputActionValue& Value);
	//2. 점프입력::Started을 처리하는 바인딩 함수
	void OnIAJumpStarted(const FInputActionValue& Value);
	//3. 공격입력::Started을 처리하는 바인딩 함수
	void OnIAAttackStarted(const FInputActionValue& Value);
	//3.b 공격입력::Triggered을 처리하는 바인딩 함수 (연속공격)
	void OnIAAttackTriggered(const FInputActionValue& Value);
	//4. 마우스XY 및 게임패드 오른쪽썸스틱 XY ::Triggered 를 처리하는 바인딩함수
	void OnIACamRotateTriggered(const FInputActionValue& Value);

protected:
	//케릭터를 Axis 벡터로 ScaleValue 강도로 이동 시킨다.
	//캐릭터 기본 이동과 관련된 부가적인 연산을 수행할 수 있다
	void MoveToAxis(const FVector& Axis, const float ScaleValue = 1.0f);

public:

	//이동불능 상태인지 확인한다, 이 상태에서는 이동 관련 행위를 못하도록 설계한다
	virtual bool IsCharacterImmobilized();
	//공격과 같은 주요 액션 행위가 가능한 상태인지 반환한다
	virtual bool CanCharacterAct();
	//케릭터상태 구조체를 반환한다
	FCharacterState* GetCharacterState();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*Called upon landing when falling, to perform actions based on the Hit result.Triggers the OnLanded event.
		* Note that movement mode is still "Falling" during this event.Current Velocity value is the velocity at the time of landing.*/
	virtual void Landed(const FHitResult& Hit) override;

	/** Called when the character's movement enters falling */
	virtual void Falling() override;
	/** Called when character's jump reaches Apex. Needs CharacterMovement->bNotifyApex = true */
	virtual void NotifyJumpApex() override;
};
