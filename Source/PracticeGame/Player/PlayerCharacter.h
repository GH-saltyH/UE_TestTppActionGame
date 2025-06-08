// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Character.h"
#include "../Actor/ATestProjectile.h"
#include "EnhancedInputSubsystems.h"		//���� �Է½ý����� ���� �ʿ�
#include "EnhancedInputComponent.h"		//BindAxis BindAction ���� ���� �ʿ��ϴ�
#include "../PlayerController/PlayerControllerTestGame.h"
#include "PlayerCharacter.generated.h"

//�÷��̾�ĳ���� ���� �α׼���
DECLARE_LOG_CATEGORY_EXTERN(PlayerLog, Log, All);

UCLASS()	//Blueprintable = ��� ����Ʈ���� Ȯ�� ����
class PRACTICEGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

private:
	//����� �޽��� ��� ���
	UPROPERTY(EditAnywhere)
	bool bDebugEnabled = false;
	//������ ���� ������ ���� ����� �����Ѵ�
	//�����Ϳ��� ������ �����ϴ�
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> mSpringArm;

	//ī�޶� ������Ʈ�� ������ ����  ����� �����Ѵ�
	//���� ���� �߿� �� �� �ִ�
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent>	mCamera;

	//�ִ��ν��Ͻ��� �޾Ƶ� ������
	TObjectPtr<class UAnimInstancePlayer>	mAnimInst;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AATestProjectile> mProjectileClass;

	//�ɸ��� ���� ������ ���� ����ü�̴�  ��Ʈ�÷��׷�  ��Ʈ�����ڸ� �����Ѵ�
	//8��Ʈ�̱� ������ �ִ� 8���� ���¸� ������ �� �ִ�
	FCharacterState	mCharacterState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	//�Է� �׼ǿ� ���� ���ε��� �Լ���
	// �ݵ�� void ��ȯŸ��
	// const FInputActionValue& Value ���� Ÿ���� �����ؾ� �Ѵ�

	//1. �̵��Է�::Triggered�� ó���ϴ� ���ε� �Լ�
	void OnIAMoveTriggered(const FInputActionValue& Value);
	//2. �����Է�::Started�� ó���ϴ� ���ε� �Լ�
	void OnIAJumpStarted(const FInputActionValue& Value);
	//3. �����Է�::Started�� ó���ϴ� ���ε� �Լ�
	void OnIAAttackStarted(const FInputActionValue& Value);
	//3.b �����Է�::Triggered�� ó���ϴ� ���ε� �Լ� (���Ӱ���)
	void OnIAAttackTriggered(const FInputActionValue& Value);
	//4. ���콺XY �� �����е� �����ʽ潺ƽ XY ::Triggered �� ó���ϴ� ���ε��Լ�
	void OnIACamRotateTriggered(const FInputActionValue& Value);

protected:
	//�ɸ��͸� Axis ���ͷ� ScaleValue ������ �̵� ��Ų��.
	//ĳ���� �⺻ �̵��� ���õ� �ΰ����� ������ ������ �� �ִ�
	void MoveToAxis(const FVector& Axis, const float ScaleValue = 1.0f);

public:

	//�̵��Ҵ� �������� Ȯ���Ѵ�, �� ���¿����� �̵� ���� ������ ���ϵ��� �����Ѵ�
	virtual bool IsCharacterImmobilized();
	//���ݰ� ���� �ֿ� �׼� ������ ������ �������� ��ȯ�Ѵ�
	virtual bool CanCharacterAct();
	//�ɸ��ͻ��� ����ü�� ��ȯ�Ѵ�
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
