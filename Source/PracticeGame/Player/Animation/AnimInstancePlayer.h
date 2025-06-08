// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "../PlayerCharacter.h"
#include "AnimInstancePlayer.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AnimInstLog, Log, All);

UCLASS()
class PRACTICEGAME_API UAnimInstancePlayer : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	bool bDebugEnabled = false;

	//�÷��̾� �ִϸ��̼��� �����ϴ� �������̴�, �������Ʈ���� ���� �� �ִ�
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EPlayerAnim		mPlayerAnim = EPlayerAnim::Ground;

	//ĳ������ �̵��ӵ��� �޾ƿ´�, �������Ʈ���� �о� �� �� �ִ�
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float mMoveSpeed = .0f;

	//�̵��� ������ �޾ƿ´� �������Ʈ���� �о�� �� �ִ�
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector mDirection = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool mIsFalling = false;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool mIsJumping = false;

	//isLanding �� true �� �ٸ� �ൿ�� �� �� ����.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool mIsLanding = false;

	//Ư�� �ൿ�� ������ �ʾҴ��� ����Ѵ�  true �� �ൿ���̴�
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool mIsInActing = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FRotator	mCamRotateByInput = FRotator(0, 0, 0);

	//��Ÿ�ָ� ������ ��� -> �������Ʈ���� ������ �� �ִ�
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage>	mAttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage>	mLandedMontage;

public:
	//��Ÿ���� ���� �̸��� ������ ���� �迭 ��� -> �������Ʈ���� ������ �� �ִ�
	UPROPERTY(EditAnywhere)
	TArray<FName>		mAttackSectionArray;

	UPROPERTY(BlueprintReadOnly)
	int32			mSectionIndex = 0;

public:	
	void SetPlayerAnim(EPlayerAnim PlayerAnim);
	void SetDirection(FVector& NewDirection);

	void SetIsJumping(bool IsJumping);
	void SetIsFalling(bool IsFalling);
	void SetIsLanding(bool IsLanding);
	void SetIsInActing(bool IsInActing);

	bool GetIsFalling();
	bool GetIsJumping();
	bool GetIsInActing();
	bool GetIsLanding();

public:
	void SetCamRotateByInput(const FRotator& NewRotator);
	void SetCamRotateByInput(float NewPitch, float NewYaw, float NewRoll = 0.f);

public:
	virtual UAnimMontage* GetAttakMontage();
	virtual UAnimMontage* GetLandMontage();
	//Ư�� ��Ÿ���� Ư�� ������ ����Ѵ�
	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;

public:
	/*��Ƽ�����Լ���
	AttackStart �� �������� �浹���� ��������
	AttackEnd �� �������� �浹���� ��������
	ComboStart �� ���� �޺��� ���� �� �ִ� ����		*/
	UFUNCTION()
	virtual void AnimNotify_AttackStart();
	UFUNCTION()
	virtual void AnimNotify_AttackEnd();
	UFUNCTION()
	virtual void AnimNotify_ComboStart();
	UFUNCTION()
	virtual void AnimNotify_ComboEnd();
	UFUNCTION()
	virtual void AnimNotify_LandRecoveryEnd();

	UFUNCTION()
	virtual void OnLandMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	virtual void OnLandBlendOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	virtual void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	virtual void OnAttackMontageBlendOut(UAnimMontage* Montage, bool bInterrupted);
};
