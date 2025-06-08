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

	//플레이어 애니메이션을 결정하는 열거형이다, 블루프린트에서 읽을 수 있다
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	EPlayerAnim		mPlayerAnim = EPlayerAnim::Ground;

	//캐릭터의 이동속도를 받아온다, 블루프린트에서 읽어 올 수 있다
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float mMoveSpeed = .0f;

	//이동할 방향을 받아온다 블루프린트에서 읽어올 수 있다
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector mDirection = FVector(0, 0, 0);

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool mIsFalling = false;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool mIsJumping = false;

	//isLanding 이 true 면 다른 행동을 할 수 없다.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool mIsLanding = false;

	//특정 행동이 끝나지 않았는지 기록한다  true 면 행동중이다
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool mIsInActing = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FRotator	mCamRotateByInput = FRotator(0, 0, 0);

	//몽타주를 가져올 멤버 -> 블루프린트에서 수정할 수 있다
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage>	mAttackMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage>	mLandedMontage;

public:
	//몽타주의 섹션 이름을 저장할 가변 배열 멤버 -> 블루프린트에서 수정할 수 있다
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
	//특정 몽타주의 특정 섹션을 재생한다
	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;

public:
	/*노티파이함수들
	AttackStart 는 프레임의 충돌판정 시작지점
	AttackEnd 는 프레임의 충돌판정 종료지점
	ComboStart 는 다음 콤보를 넣을 수 있는 지점		*/
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
