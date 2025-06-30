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
	
//데이터 테이블 관련 요소
protected:
	//템플릿을 상속받는 객체의 실제 애님정보가 들어있는  데이터 테이블
	TObjectPtr<UDataTable>	mPlayerAnimDataTable;

	//FCharacterState* mCharacterState;
protected:
	UPROPERTY(EditAnywhere)
	FName				mAnimName;

	UPROPERTY(EditAnywhere)
	EPlayerJob          mJob;

	//시퀀스플레이어
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, TObjectPtr<UAnimSequence>>  mAnimMap;

	//이동블렌드스페이스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UBlendSpace>     mGroundBlendSpace;

	//에임오프셋
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAimOffsetBlendSpace>    mViewAimOffSet;

	//공격몽타주
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage>        mAttackMontage;

	//공격몽타주섹션이름
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FName>                           mAttackSectionArray;

	//랜드몽타주
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage>			mLandMontage;

	//대시를Additive 로 하려할때 알파
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float						mDashAlpha = 0.f;

protected:
	//타이머 핸들러, 스킬은 여러개가 있을 수 있어 TArray 로 관리
	FTimerHandle		mCleaveEndTimer;
	FTimerHandle		mDashEndTimer;
	FTimerHandle		mDashAfterImageEndTimer;
	FTimerHandle		mSpecialEndTimer;
	TArray<FTimerHandle>	mSkillEndTimer;

public:
	//블루프린트에서 호출가능하게 설정
	//애님 데이터를 지정해주는 기능을 한다
	UFUNCTION(BlueprintCallable)
	void SetAnimData(const FName& Name);

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

	////몽타주를 가져올 멤버 -> 블루프린트에서 수정할 수 있다
	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UAnimMontage>	mAttackMontage;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UAnimMontage>	mLandedMontage;

//public:
//	//몽타주의 섹션 이름을 저장할 가변 배열 멤버 -> 블루프린트에서 수정할 수 있다
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
	//인자로 받은 애님타입에 따라 타이머를 설정한다
	//노티파이가 호출이 보장되지 않기 때문에
	//타이머를 통해 애니메이션의 길이에 해당하는 시간에 플래그를 처리하게 만든다
	void SetAnimTimerHandle(FName KeyName, EPlayerAnim AnimType, uint8 CurrentState);
	void ClearDashAfterImageTimer();

	void ClearStatesByTimerHandle()
	{
		//애님상태를 일반 그라운드 상태로 바꾼다
		mPlayerAnim = EPlayerAnim::Ground;
		//액팅 상태를 해제한다
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
	//특정 몽타주의 특정 섹션을 재생한다
	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	//들고 있는 애님맵의 특정 애니메이션의 재생위치를 리셋해준다
	void ResetPlayerAnimSeq(const FName AnimName);

public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeInitializeAnimation() override;
	virtual void PostInitProperties() override;

public:
	/*노티파이함수들  -> 몇몇 노티파이는 타이머로 대체한다
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
