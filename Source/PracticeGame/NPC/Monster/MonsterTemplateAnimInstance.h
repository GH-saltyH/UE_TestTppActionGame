// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "MonsterTemplateAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UMonsterTemplateAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMonsterTemplateAnimInstance();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EMonsterDefaultAnim		mAnimType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence>	mIdle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence>	mWalk;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence>	mRun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence>	mAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence>	mDeath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence>	mSpecial01;


	//��Ʈ �� ����Ʈ ó���� ���� ������ -> Additive �� �����Ѵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence>	mHit;

	//��Ʈ ����Ʈ�� �����ϱ� ���� ���� ��
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float						mHitAlpha = 0.f;


public:
	void ChangeAnim(EMonsterDefaultAnim Anim)
	{
		mAnimType = Anim;
	}

	void Hit()
	{
		mHitAlpha = 1.0f;
	}

public:
	UFUNCTION()
	void AnimNotify_Attack();

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_Death();

	//Ư�� �ɷ��� ��Ƽ���̸� ������ �� 
	UFUNCTION()
	void AnimNotify_Special();

	UFUNCTION()
	void AnimNotify_HitEnd();
};
