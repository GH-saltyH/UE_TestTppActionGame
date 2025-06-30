// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstancePlayer.h"
//�α� ī�װ� ����
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
		//��Ÿ�ָ� ����Ѵ�
		Montage_Play(Montage);
		//����ϴ� ��Ÿ���� ������ ������Ų��
		Montage_JumpToSection(SectionName, Montage);
}

void UAnimInstancePlayer::NativeBeginPlay()
{
	Super::NativeBeginPlay();

}

void UAnimInstancePlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//�� �ִ��ν��Ͻ��� ������ �ִ� ���̷�Ż �޽ø�
	// ������ �ִ� ���� ������ �� �� ĳ���� Ŭ������ ĳ�����Ѵ�
	//PC ��ü�� �޾ƿ´�
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	//ĳ���ÿ� �����ϸ�
	if (IsValid(PC))
	{
		//ĳ���� Ŭ������ �����Ʈ������Ʈ�� ���´�
		UCharacterMovementComponent* CMovement = PC->GetCharacterMovement();

		//�����Ʈ������Ʈ ȹ�濡 �����ϸ� ���� �̵��ӵ��� ���´�
		if (IsValid(CMovement))
		{
				//�̵��ӵ� = 
				// �����Ʈ ������Ʈ ���� 
				//		FVector  Velocity �� ũ�� 
			mMoveSpeed = CMovement->Velocity.Length();
			//�׶��忡�� �̵� �� �϶��� 
			if (CMovement->IsMovingOnGround() && mMoveSpeed>=50.f)
			{		
				//�ִϸ��̼� �÷��׸� MoveXY �� ����
				mPlayerAnim = EPlayerAnim::MoveXY;
				mIsFalling = false;
				if(bDebugEnabled)
				UE_LOG(AnimInstLog, Display, TEXT("Character is Moving. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
			//�̵����� �ƴϰ� Falling ���϶� ó��
			else if (CMovement->IsFalling())
			{
				//�ִϸ��̼� �÷��׸� Falling ���� ó���Ѵ�
				mIsFalling = true;
				mPlayerAnim = EPlayerAnim::Falling;
				if (bDebugEnabled)
				UE_LOG(AnimInstLog, Display, TEXT("Character is Falling. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
			//�ٴڿ� �ְ� �̵��� ���� �� 
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
	//������Ÿ�� ���� ��������Ʈ ���ε�
	if (mLandedMontage)
	{
		//��Ÿ�ֿ��� �� ��������Ʈ �Լ�
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUFunction(this, FName("OnLandMontageEnd"));
		Montage_SetEndDelegate(MontageEndDelegate, mLandedMontage);

		//��Ÿ�� ���̵���� �� ��������Ʈ �Լ�
		FOnMontageBlendingOutStarted MontageBlendOutDelegate;
		MontageBlendOutDelegate.BindUFunction(this, FName("OnLandBlendOut"));
		Montage_SetBlendingOutDelegate(MontageBlendOutDelegate, mLandedMontage);
	}

	//���ݸ�Ÿ�� ���� ��������Ʈ ���ε�
	if (mAttackMontage)
	{
		//��Ÿ�ֿ��� �� ��������Ʈ �Լ�
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUFunction(this, FName("OnAttackMontageEnd"));
		Montage_SetEndDelegate(MontageEndDelegate, mAttackMontage);

		//��Ÿ�� ���̵���� �� ��������Ʈ �Լ�
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
	//�÷��� �浹 ON : �̱���
}

void UAnimInstancePlayer::AnimNotify_AttackEnd()
{
	UE_LOG(AnimInstLog, Display, TEXT("Notify AttackEnd Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//�÷��� �浹 OFF : �̱���
}

void UAnimInstancePlayer::AnimNotify_ComboStart()
{
	UE_LOG(AnimInstLog, Display, TEXT("Notify ComboStart Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//�÷��� �޺� ON 
	StatePtr->SetState(StatePtr->COMBO);
	//�÷��� ���� OFF -> �������� ���ݵ��� ACTING ���� ������ ��������
	StatePtr->ClearState(StatePtr->ACTING);
}

void UAnimInstancePlayer::AnimNotify_ComboEnd()
{
	UE_LOG(AnimInstLog, Display, TEXT("Notify ComboEnd Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//�÷��� �޺� ON 
	StatePtr->ClearState(StatePtr->COMBO);
}

void UAnimInstancePlayer::AnimNotify_LandRecoveryEnd()
{
	UE_LOG(AnimInstLog, Display, TEXT("Notify LandEnd Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();

	//���� ��� �Ϸ� : LANDING �� �����Ѵ�
	//�ٸ� �ִϸ��̼��� �̰����� �����ϱ� ���� ���� �� �ֱ� ������ 
	//EndDelegate �Լ��� �ϳ� �� ���ε��صд�
	//OnLandMontageEnd
	StatePtr->ClearState(StatePtr->LANDING);
	StatePtr->ClearState(StatePtr->ACTING);
}

void UAnimInstancePlayer::OnLandMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	//��������Ʈ ���ε��� �Լ��̴�
	//���� ����� �ǵ����� ������ ������ (�ַ� �ִԱ׷����� ���� ���⼺�� ����)
	
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//������������ �̵��Ұ� ���¸� �����Ѵ�
	StatePtr->ClearState(StatePtr->LANDING);
	StatePtr->ClearState(StatePtr->ACTING);
	UE_LOG(AnimInstLog, Warning, TEXT("OnLandMontageEnd Called"));
}

void UAnimInstancePlayer::OnLandBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	//��������Ʈ ���ε��� �Լ��̴�
	//���� �ƿ��� ���� �̵��Ұ� ���¸� �������ش�
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//�������� ���� �̵��Ұ� ���¸� �����Ѵ�
	StatePtr->ClearState(StatePtr->LANDING);
	StatePtr->ClearState(StatePtr->ACTING);
	UE_LOG(AnimInstLog, Warning, TEXT("OnLandBlendOut Called"));
}

void UAnimInstancePlayer::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	//��������Ʈ ���ε��� �Լ��̴�
	//���� ��Ÿ���� ����� ���� �� �� ȣ��
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//�޺� ���� ���� 
	//State->ClearState(State->COMBO);
	//���� �� ����
	StatePtr->ClearState(StatePtr->ACTING);
	UE_LOG(TestGame, Warning, TEXT("OnAttackMontageEnd has been called"));
}

void UAnimInstancePlayer::OnAttackMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	//��������Ʈ ���ε��� �Լ��̴�
	//���� ��Ÿ�ְ� ���� �ƿ� �� �� ȣ��
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	//�޺� ���� ���� 
	//State->ClearState(State->COMBO);
	//������ ����
	StatePtr->ClearState(StatePtr->ACTING);
	UE_LOG(TestGame, Warning, TEXT("OnAttackMontageBlendOut has been called"));
}
