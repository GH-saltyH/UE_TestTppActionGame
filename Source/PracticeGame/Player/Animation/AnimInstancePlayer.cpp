// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstancePlayer.h"
//로그 카테고리 설정
DEFINE_LOG_CATEGORY(AnimInstLog);

void UAnimInstancePlayer::SetPlayerAnim(EPlayerAnim PlayerAnim)
{
		mPlayerAnim = PlayerAnim;
}

void UAnimInstancePlayer::SetDirection(FVector& NewDirection)
{
	mDirection = NewDirection;
	/*UE_LOG(AnimInstLog, Display,
		TEXT("Set mDirection to NewDirection")
	); */
	if(bDebugEnabled)
	UE_LOG(AnimInstLog, Display, TEXT("mDirection = ( %.1f, %.1f, %.1f )"), mDirection.X, mDirection.Y, mDirection.Z);
}

void UAnimInstancePlayer::SetIsJumping(bool IsJumping)
{
	mIsJumping = IsJumping;
}

void UAnimInstancePlayer::SetIsFalling(bool IsFalling)
{
	mIsFalling = IsFalling;
}

void UAnimInstancePlayer::SetIsLanding(bool IsLanding)
{
	mIsLanding = IsLanding;
}

void UAnimInstancePlayer::SetIsInActing(bool IsInActing)
{
	mIsInActing = IsInActing;
}

bool UAnimInstancePlayer::GetIsFalling()
{
	return mIsFalling;
}

bool UAnimInstancePlayer::GetIsJumping()
{
	return mIsJumping;
}

bool UAnimInstancePlayer::GetIsInActing()
{
	return mIsInActing;
}

bool UAnimInstancePlayer::GetIsLanding()
{
	return mIsLanding;
}

void UAnimInstancePlayer::SetCamRotateByInput(const FRotator& NewRotator)
{
	mCamRotateByInput = NewRotator;
}

void UAnimInstancePlayer::SetCamRotateByInput(float NewPitch, float NewYaw, float NewRoll)
{
	mCamRotateByInput.Pitch	= NewPitch;
	mCamRotateByInput.Yaw	= NewYaw;
	mCamRotateByInput.Roll	= NewRoll;
}

UAnimMontage* UAnimInstancePlayer::GetAttakMontage()
{
	if (!mAttackMontage)
		return nullptr;

	return mAttackMontage;
}

UAnimMontage* UAnimInstancePlayer::GetLandMontage()
{
	if (!mLandedMontage)
		return nullptr;

	return mLandedMontage;
}

void UAnimInstancePlayer::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
		//몽타주를 재생한다
		Montage_Play(Montage);
		//재생하는 몽타주의 섹션을 점프시킨다
		Montage_JumpToSection(SectionName, Montage);
}

void UAnimInstancePlayer::NativeBeginPlay()
{
	Super::NativeBeginPlay();

}

void UAnimInstancePlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//이 애님인스턴스를 가지고 있는 스켈레탈 메시를
	// 가지고 있는 폰을 가지고 온 후 캐릭터 클래스로 캐스팅한다
	//PC 객체에 받아온다
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	//캐스팅에 성공하면
	if (IsValid(PC))
	{
		//캐릭터 클래스의 무브먼트컴포넌트를 얻어온다
		UCharacterMovementComponent* CMovement = PC->GetCharacterMovement();

		//무브먼트컴포넌트 획득에 성공하면 현재 이동속도를 얻어온다
		if (IsValid(CMovement))
		{
				//이동속도 = 
				// 무브먼트 컴포넌트 내의 
				//		FVector  Velocity 의 크기 
			mMoveSpeed = CMovement->Velocity.Length();
			//그라운드에서 이동 중 일때만 
			if (CMovement->IsMovingOnGround() && mMoveSpeed>=50.f)
			{		
				//애니메이션 플래그를 MoveXY 로 설정
				mPlayerAnim = EPlayerAnim::MoveXY;
				mIsFalling = false;
				if(bDebugEnabled)
				UE_LOG(AnimInstLog, Display, TEXT("Character is Moving. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
			//이동중이 아니고 Falling 중일때 처리
			else if (CMovement->IsFalling())
			{
				//애니메이션 플래그를 Falling 으로 처리한다
				mIsFalling = true;
				mPlayerAnim = EPlayerAnim::Falling;
				if (bDebugEnabled)
				UE_LOG(AnimInstLog, Display, TEXT("Character is Falling. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
			//바닥에 있고 이동이 없을 때 
			else if (CMovement->IsMovingOnGround() && mMoveSpeed < 50.f)
			{
				mIsFalling = false;
				mPlayerAnim = EPlayerAnim::Ground;
				if (bDebugEnabled)
				UE_LOG(AnimInstLog, Display, TEXT("Character is on ground. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
		}
	}
}

void UAnimInstancePlayer::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//착지몽타주 관련 델리게이트 바인딩
	if (mLandedMontage)
	{
		//몽타주엔드 시 델리게이트 함수
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUFunction(this, FName("OnLandMontageEnd"));
		Montage_SetEndDelegate(MontageEndDelegate, mLandedMontage);

		//몽타주 페이드오프 시 델리게이트 함수
		FOnMontageBlendingOutStarted MontageBlendOutDelegate;
		MontageBlendOutDelegate.BindUFunction(this, FName("OnLandBlendOut"));
		Montage_SetBlendingOutDelegate(MontageBlendOutDelegate, mLandedMontage);
	}

	//공격몽타주 관련 델리게이트 바인딩
	if (mAttackMontage)
	{
		//몽타주엔드 시 델리게이트 함수
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUFunction(this, FName("OnAttackMontageEnd"));
		Montage_SetEndDelegate(MontageEndDelegate, mAttackMontage);

		//몽타주 페이드오프 시 델리게이트 함수
		FOnMontageBlendingOutStarted MontageBlendOutDelegate;
		MontageBlendOutDelegate.BindUFunction(this, FName("OnAttackMontageBlendOut"));
		Montage_SetBlendingOutDelegate(MontageBlendOutDelegate, mAttackMontage);
	}
}

void UAnimInstancePlayer::AnimNotify_AttackStart()
{
	UE_LOG(AnimInstLog, Display, TEXT("Notify AttackStart Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//플래그 충돌 ON : 미구현
}

void UAnimInstancePlayer::AnimNotify_AttackEnd()
{
	UE_LOG(AnimInstLog, Display, TEXT("Notify AttackEnd Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//플래그 충돌 OFF : 미구현
}

void UAnimInstancePlayer::AnimNotify_ComboStart()
{
	UE_LOG(AnimInstLog, Display, TEXT("Notify ComboStart Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//플래그 콤보 ON 
	StatePtr->SetState(StatePtr->COMBO);
	//플래그 액팅 OFF -> 이제부터 공격등의 ACTING 유형 행위가 가능해짐
	StatePtr->ClearState(StatePtr->ACTING);
}

void UAnimInstancePlayer::AnimNotify_ComboEnd()
{
	UE_LOG(AnimInstLog, Display, TEXT("Notify ComboEnd Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//플래그 콤보 ON 
	StatePtr->ClearState(StatePtr->COMBO);
}

void UAnimInstancePlayer::AnimNotify_LandRecoveryEnd()
{
	UE_LOG(AnimInstLog, Display, TEXT("Notify LandEnd Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();

	//착지 모션 완료 : LANDING 를 해제한다
	//다른 애니메이션이 이과정에 도달하기 전에 덮을 수 있기 떄문에 
	//EndDelegate 함수를 하나 더 바인딩해둔다
	//OnLandMontageEnd
	StatePtr->ClearState(StatePtr->LANDING);
	StatePtr->ClearState(StatePtr->ACTING);
}

void UAnimInstancePlayer::OnLandMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	//델리게이트 바인딩된 함수이다
	//착지 모션이 의도보다 빠르게 끝나도 (주로 애님그래프의 조건 복잡성에 의해)
	
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//렌딩으로인한 이동불가 상태를 해제한다
	StatePtr->ClearState(StatePtr->LANDING);
	StatePtr->ClearState(StatePtr->ACTING);
	UE_LOG(AnimInstLog, Warning, TEXT("OnLandMontageEnd Called"));
}

void UAnimInstancePlayer::OnLandBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	//델리게이트 바인딩된 함수이다
	//블렌드 아웃될 때도 이동불가 상태를 해제해준다
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//렌딩으로 인한 이동불가 상태를 해제한다
	StatePtr->ClearState(StatePtr->LANDING);
	StatePtr->ClearState(StatePtr->ACTING);
	UE_LOG(AnimInstLog, Warning, TEXT("OnLandBlendOut Called"));
}

void UAnimInstancePlayer::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	//델리게이트 바인딩된 함수이다
	//공격 몽타주의 재생이 종료 될 때 호출
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//콤보 가능 해제 
	//State->ClearState(State->COMBO);
	//액팅 중 해제
	StatePtr->ClearState(StatePtr->ACTING);
	UE_LOG(TestGame, Warning, TEXT("OnAttackMontageEnd has been called"));
}

void UAnimInstancePlayer::OnAttackMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	//델리게이트 바인딩된 함수이다
	//공격 몽타주가 블렌드 아웃 될 때 호출
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//콤보 가능 해제 
	//State->ClearState(State->COMBO);
	//액팅중 해제
	StatePtr->ClearState(StatePtr->ACTING);
	UE_LOG(TestGame, Warning, TEXT("OnAttackMontageBlendOut has been called"));
}
