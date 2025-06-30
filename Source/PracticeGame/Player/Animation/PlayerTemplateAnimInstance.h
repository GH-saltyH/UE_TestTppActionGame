// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "../PlayerCharacter.h"
#include "PlayerTemplateAnimInstance.generated.h"

UCLASS()
class PRACTICEGAME_API UPlayerTemplateAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerTemplateAnimInstance();
	
//������ ���̺� ���� ���
protected:
	//���ø��� ��ӹ޴� ��ü�� ���� �ִ������� ����ִ�  ������ ���̺�
	TObjectPtr<UDataTable>	mPlayerAnimDataTable;

	//FCharacterState* mCharacterState;
protected:
	UPROPERTY(EditAnywhere)
	FName				mAnimName;

	UPROPERTY(EditAnywhere)
	EPlayerJob          mJob;

	//�������÷��̾�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, TObjectPtr<UAnimSequence>>  mAnimMap;

	//�̵����彺���̽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBlendSpace>     mGroundBlendSpace;

	//���ӿ�����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAimOffsetBlendSpace>    mViewAimOffSet;

	//���ݸ�Ÿ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage>        mAttackMontage;

	//���ݸ�Ÿ�ּ����̸�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FName>                           mAttackSectionArray;

	//�����Ÿ��
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage>			mLandMontage;

	//��ø�Additive �� �Ϸ��Ҷ� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float						mDashAlpha = 0.f;

protected:
	//Ÿ�̸� �ڵ鷯, ��ų�� �������� ���� �� �־� TArray �� ����
	FTimerHandle		mCleaveEndTimer;
	FTimerHandle		mDashEndTimer;
	FTimerHandle		mDashAfterImageEndTimer;
	FTimerHandle		mSpecialEndTimer;
	TArray<FTimerHandle>	mSkillEndTimer;

public:
	//�������Ʈ���� ȣ�Ⱑ���ϰ� ����
	//�ִ� �����͸� �������ִ� ����� �Ѵ�
	UFUNCTION(BlueprintCallable)
	void SetAnimData(const FName& Name);

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

	////��Ÿ�ָ� ������ ��� -> �������Ʈ���� ������ �� �ִ�
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UAnimMontage>	mAttackMontage;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UAnimMontage>	mLandedMontage;

//public:
//	//��Ÿ���� ���� �̸��� ������ ���� �迭 ��� -> �������Ʈ���� ������ �� �ִ�
//	UPROPERTY(EditAnywhere)
//	TArray<FName>		mAttackSectionArray;

	UPROPERTY(BlueprintReadOnly)
	int32			mSectionIndex = 0;

public:
	void SetPlayerAnim(EPlayerAnim PlayerAnim);
	EPlayerAnim GetPlayerAnim();
	bool HasPlayerAnim(EPlayerAnim HasThisAnim);

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
	//���ڷ� ���� �ִ�Ÿ�Կ� ���� Ÿ�̸Ӹ� �����Ѵ�
	//��Ƽ���̰� ȣ���� ������� �ʱ� ������
	//Ÿ�̸Ӹ� ���� �ִϸ��̼��� ���̿� �ش��ϴ� �ð��� �÷��׸� ó���ϰ� �����
	void SetAnimTimerHandle(FName KeyName, EPlayerAnim AnimType, uint8 CurrentState);
	void ClearDashAfterImageTimer();

	void ClearStatesByTimerHandle()
	{
		//�ִԻ��¸� �Ϲ� �׶��� ���·� �ٲ۴�
		mPlayerAnim = EPlayerAnim::Ground;
		//���� ���¸� �����Ѵ�
		ClearCharacterState(FCharacterState::ACTING);
		UE_LOG(TestGame, Warning, TEXT("Skil Timer Activated"));
	}

	void PlayAttack();
	void PlayDash();
	void PlaySpecial();
	void PlayCleave();

	bool HasCharacterState(uint8 State);
	void SetCharacterState(uint8 NewState);
	void ClearCharacterState(uint8 State);

public:
	virtual UAnimMontage* GetAttakMontage();
	virtual UAnimMontage* GetLandMontage();
	//Ư�� ��Ÿ���� Ư�� ������ ����Ѵ�
	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	//��� �ִ� �ִԸ��� Ư�� �ִϸ��̼��� �����ġ�� �������ش�
	void ResetPlayerAnimSeq(const FName AnimName);

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;
	virtual void PostInitProperties() override;

public:
	/*��Ƽ�����Լ���  -> ��� ��Ƽ���̴� Ÿ�̸ӷ� ��ü�Ѵ�
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
	virtual void AnimNotify_DashEnd();
	UFUNCTION()
	virtual void AnimNotify_SpecialEnd();

	UFUNCTION()
	virtual void OnLandMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	virtual void OnLandBlendOut(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	virtual void OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	virtual void OnAttackMontageBlendOut(UAnimMontage* Montage, bool bInterrupted);


};
