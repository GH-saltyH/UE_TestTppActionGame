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


	//히트 시 이펙트 처리를 위한 시퀀스 -> Additive 로 블렌딩한다
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimSequence>	mHit;

	//히트 이펙트를 블렌딩하기 위한 알파 값
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

	//특수 능력의 노티파이를 실행할 때 
	UFUNCTION()
	void AnimNotify_Special();

	UFUNCTION()
	void AnimNotify_HitEnd();
};
