// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerTemplateAnimInstance.h"

UPlayerTemplateAnimInstance::UPlayerTemplateAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable>
		PlayerAnimTable(TEXT(
			"/Script/Engine.DataTable'/Game/Player/DTPlayerAnimData.DTPlayerAnimData'"
		));

	if (PlayerAnimTable.Succeeded())
		mPlayerAnimDataTable = PlayerAnimTable.Object;

}

void UPlayerTemplateAnimInstance::SetAnimData(const FName& Name)
{
	//Name �������� �ϳ��� �о�´�
	const		FPlayerAnimData* AnimData =
		mPlayerAnimDataTable->FindRow<FPlayerAnimData>(Name, TEXT(""));

	//�о����  ����� �޾Ƶд�
	if (AnimData)
	{
		mJob=AnimData->Job;
		mAnimMap = AnimData->AnimMap;
		mGroundBlendSpace = AnimData->GroundBlendSpace;
		mViewAimOffSet = AnimData->ViewAimOffSet;
		mAttackMontage=AnimData->AttackMontage;
		mAttackSectionArray = AnimData->AttackSectionArray;
		mLandMontage = AnimData->LandMontage;
	}
}

void UPlayerTemplateAnimInstance::SetPlayerAnim(EPlayerAnim PlayerAnim)
{
	mPlayerAnim = PlayerAnim;
}

EPlayerAnim UPlayerTemplateAnimInstance::GetPlayerAnim()
{
	return mPlayerAnim;
}

bool UPlayerTemplateAnimInstance::HasPlayerAnim(EPlayerAnim HasThisAnim)
{
	return HasThisAnim == mPlayerAnim;
}

void UPlayerTemplateAnimInstance::SetDirection(FVector& NewDirection)
{
	mDirection = NewDirection;
	/*UE_LOG(TestGame, Display,
		TEXT("Set mDirection to NewDirection")
	); */
	if (bDebugEnabled)
		UE_LOG(TestGame, Display, TEXT("mDirection = ( %.1f, %.1f, %.1f )"), mDirection.X, mDirection.Y, mDirection.Z);
}

void UPlayerTemplateAnimInstance::SetIsJumping(bool IsJumping)
{
	mIsJumping = IsJumping;
}

void UPlayerTemplateAnimInstance::SetIsFalling(bool IsFalling)
{
	mIsFalling = IsFalling;
}

void UPlayerTemplateAnimInstance::SetIsLanding(bool IsLanding)
{
	mIsLanding = IsLanding;
}

void UPlayerTemplateAnimInstance::SetIsInActing(bool IsInActing)
{
	mIsInActing = IsInActing;
}

bool UPlayerTemplateAnimInstance::GetIsFalling()
{
	return mIsFalling;
}

bool UPlayerTemplateAnimInstance::GetIsJumping()
{
	return mIsJumping;
}

bool UPlayerTemplateAnimInstance::GetIsInActing()
{
	return mIsInActing;
}

bool UPlayerTemplateAnimInstance::GetIsLanding()
{
	return mIsLanding;
}

void UPlayerTemplateAnimInstance::SetCamRotateByInput(const FRotator& NewRotator)
{
	mCamRotateByInput = NewRotator;
}

void UPlayerTemplateAnimInstance::SetCamRotateByInput(float NewPitch, float NewYaw, float NewRoll)
{
	mCamRotateByInput.Pitch = NewPitch;
	mCamRotateByInput.Yaw = NewYaw;
	mCamRotateByInput.Roll = NewRoll;
}

void UPlayerTemplateAnimInstance::SetAnimTimerHandle(FName KeyName, EPlayerAnim AnimType, uint8 CurrentState)
{
	//if (!IsValid(GetWorld()))
	//{
	//	// �����ϰ� ���� �Ǵ� ��ü ó��
	//	return;
	//}
	//mAnimMap ���� �������� ã�� �����ϸ� �ش� �������� ����ð��� ������ Ÿ�̸Ӹ� �����Ѵ�
	//Ÿ�̸Ӵ� �����ð� ���� �÷��׸� �ǵ����� �ٽ� �ൿ�� �����Ӱ� �� �� �ְ� ���ش�
	if (TObjectPtr<UAnimSequence>* FoundSequence = mAnimMap.Find(KeyName))
	{
		if (*FoundSequence)
		{
			UWorld* World = (GetOwningComponent() ? GetOwningComponent()->GetWorld() : nullptr);
			if (World)
			{
				//�������� ����ð��� ���´�
				float AnimDuration = (*FoundSequence)->GetPlayLength();
				//����ð� ���� ���� �÷��� ���� �Լ��� ���۽�Ų��
				switch (AnimType)
				{
				case EPlayerAnim::Cleave:
					World->GetTimerManager().SetTimer(
						mCleaveEndTimer,
						this,
						&UPlayerTemplateAnimInstance::ClearStatesByTimerHandle,
						AnimDuration,
						false);
					break;
				case EPlayerAnim::Dash:
					World->GetTimerManager().SetTimer(
						mDashEndTimer,
						this,
						&UPlayerTemplateAnimInstance::ClearStatesByTimerHandle,
						AnimDuration,
						false);
					World->GetTimerManager().SetTimer(
						mDashAfterImageEndTimer,
						this,
						&UPlayerTemplateAnimInstance::ClearDashAfterImageTimer,
						AnimDuration,
						false);
					break;
				case EPlayerAnim::Special:
					World->GetTimerManager().SetTimer(
						mSpecialEndTimer,
						this,
						&UPlayerTemplateAnimInstance::ClearStatesByTimerHandle,
						AnimDuration,
						false);
					break;
				}
			}
		}
		else
		{
			//�������� ������ �÷��׵��� ��� �����Ѵ�
			mPlayerAnim = EPlayerAnim::Ground;
			ClearCharacterState(CurrentState);
		}
	}
}

void UPlayerTemplateAnimInstance::ClearDashAfterImageTimer()
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (IsValid(PC))
	{
		if (PC->mDashAfterImageTimerHandler.IsValid())
		{
			//if (!IsValid(GetWorld()))
			//{
			//	// �����ϰ� ���� �Ǵ� ��ü ó��
			//	return;
			//}
			GetWorld()->GetTimerManager().ClearTimer(PC->mDashAfterImageTimerHandler);
			UE_LOG(TestGame, Warning, TEXT("Dash After Effect Timer Cleared by end of Animation frame"));
		}
	}
}

void UPlayerTemplateAnimInstance::PlayAttack()
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (IsValid(PC))
	{
		//���� ���� ���� �� �ɸ� �� �ִ� ���� ���Ѱ�ӵ� ������ �ӽ� �ذ� (�߻� ���ɼ� ����)
		FCharacterState* StatePtr = PC->GetCharacterStatePtr();
		ClearCharacterState(FCharacterState::LANDING);
		//StatePtr->ClearState(StatePtr->LANDING);


		//���� �ִ��ν��Ͻ����� ���ݸ�Ÿ�ָ� �޾ƿ��� �����ϸ� ����
		//�Լ��κ��� �÷��̾�� AnimInst �� �̵��߱� ������ �ڼ��� �׳� �ҷ����� �ȴ�.
		/*UAnimMontage* AttackMontage = GetAttakMontage();*/
		if (mAttackMontage)
		{
			//�÷��׸� �����Ѵ� ACTING : ON  -- ���������� ���� ������ ���Ѵ�
			SetCharacterState(FCharacterState::ACTING);
				//StatePtr->SetState(StatePtr->ACTING);

			//�޺� ������ ��� �ε��� ��ȯ ó��
			if (HasCharacterState(FCharacterState::COMBO))
			{
				//���� ��Ÿ�� ������ �ε����� ��ȯ �����Ѵ�
				mSectionIndex
					= (mSectionIndex + 1) % mAttackSectionArray.Num();
				//���� ����� ��Ÿ�� �̸��Ҵ�(����� ��¿�)
				//FName NextSectionName = mAttackSectionArray[mSectionIndex];
			}
			//�޺��� �ƴѰ�� �ε��� �ʱ�ȭ
			else
			{
				mSectionIndex = 0;
			}

			//���� �ε����� Ȱ���Ͽ� ����Ϸ��� ���ݼ����� �̸��� �޾ƿ´�
			FName SectionName = mAttackSectionArray[mSectionIndex];

			//Ŀ�����Լ�: ���� ��Ÿ�ֿ��� 
			//���� �Ҵ�� ������ �̸��� ����Ͽ� �� ������ �ٷ� �����Ų�� 
			PlayMontageSection(mAttackMontage, SectionName);

			//�������� ���� ��� ��Ÿ�� �̸��� �ε��� 
			if(bDebugEnabled)
			UE_LOG(TestGame, Display,
				TEXT("Current Playing Montage [%s],\tIndex[%d]"),
				*SectionName.ToString(),
				mSectionIndex);

			//���� ������ ��ġ�� ȸ������ ���߱� ���� ���� ������
			FVector SpawnLocation = PC->GetActorLocation() + PC->GetActorForwardVector() * 100.0f;
			FRotator SpawnRotation = PC->GetActorRotation();

			//AATestProjectile Ŭ������ ����ü�� ������
			AATestProjectile* Projectile = GetWorld()->SpawnActor<AATestProjectile>(PC->GetProjectileClass(), SpawnLocation, SpawnRotation);
			if (bDebugEnabled)
				UE_LOG(PlayerLog, Display, TEXT("Character : Attack"));
		}
		else
			UE_LOG(TestGame, Warning, TEXT("No Montage for this action exists"));
	}
}

void UPlayerTemplateAnimInstance::PlayDash()
{
	//�÷��� ����
	mDashAlpha = 1.0f;
	//�Ʒ� �÷��װ� �Ҵ�Ǹ� ����� ĳ�õ� ������Ʈ�ӽ��� ���õȴ�
	mPlayerAnim = EPlayerAnim::Dash;
	//����� �׻� ��µ� �� �ֵ��� �÷��̹� �������� �ʱ�ȭ�Ѵ�
	

	//�ִϸ��̼� �ð� ���� �÷��׸� ������ Ÿ�̸Ӹ� �����Ѵ�
	SetAnimTimerHandle(TEXT("Dash"),
		EPlayerAnim::Dash,
		FCharacterState::ACTING);
	
	UE_LOG(TestGame, Warning, TEXT("AnimInst : PlayDash has been called"));
}

void UPlayerTemplateAnimInstance::PlaySpecial()
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (IsValid(PC))
	{
		FCharacterState* StatePtr = PC->GetCharacterStatePtr();
		if (StatePtr)
		{
			//���伣 �����¿� ���õ� �÷��׵��� �����Ѵ�
			mPlayerAnim = EPlayerAnim::Special;

			//Ÿ�̸Ӹ� �����Ѵ�
			SetAnimTimerHandle(TEXT("Special"),
				EPlayerAnim::Special,
				FCharacterState::ACTING);
			UE_LOG(TestGame, Warning, TEXT("AnimInst : PlaySpecial has been called"));
		}
	}
	else
	{
	}
}

void UPlayerTemplateAnimInstance::PlayCleave()
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (IsValid(PC))
	{
		FCharacterState* StatePtr = PC->GetCharacterStatePtr();
		if (StatePtr)
		{
			//Cleave �����¿� ���õ� �÷��׵��� �����Ѵ�
			mPlayerAnim = EPlayerAnim::Cleave;

			//Ÿ�̸Ӹ� �����Ѵ�
			SetAnimTimerHandle(TEXT("Cleave"),
				EPlayerAnim::Cleave,
				FCharacterState::ACTING);
			UE_LOG(TestGame, Warning, TEXT("AnimInst : PlayCleave has been called"));
		}
	}
	else
	{
	}
}

bool UPlayerTemplateAnimInstance::HasCharacterState(uint8 State)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	if (StatePtr)
		return StatePtr->HasState(State);
	return false;
}

void UPlayerTemplateAnimInstance::SetCharacterState(uint8 NewState)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	if (IsValid(PC))
	{
		FCharacterState* StatePtr = PC->GetCharacterStatePtr();
		if (StatePtr)
			StatePtr->SetState(NewState);
	}
}

void UPlayerTemplateAnimInstance::ClearCharacterState(uint8 State)
{
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	FCharacterState* StatePtr = PC->GetCharacterStatePtr();
	if (StatePtr)
		StatePtr->ClearState(State);
}


UAnimMontage* UPlayerTemplateAnimInstance::GetAttakMontage()
{
	if (!mAttackMontage)
		return nullptr;

	return mAttackMontage;
}

UAnimMontage* UPlayerTemplateAnimInstance::GetLandMontage()
{
	if (!mLandMontage)
		return nullptr;

	return mLandMontage;
}

void UPlayerTemplateAnimInstance::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	//��Ÿ�ָ� ����Ѵ�
	Montage_Play(Montage);
	//����ϴ� ��Ÿ���� ������ ������Ų��
	Montage_JumpToSection(SectionName, Montage);
}

void UPlayerTemplateAnimInstance::ResetPlayerAnimSeq(const FName AnimName)
{
	//��ȿ�� �˻�
	//�����ϰ� ó���ϱ� ���� ��ȿ���� �˻��Ѵ�
	if (mAnimMap.Contains(AnimName))
	{
		UAnimSequence* AnimSeq = mAnimMap[AnimName];
		if (AnimSeq)
		{
			//���� �ִ� ������ �÷��̾ ã�Ƽ� �����Ѵ�
			StopAllMontages(0.0f);		//��Ÿ�ֵ��� ��� �ߴܽ�Ų��
		}
	}
}

void UPlayerTemplateAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

}

void UPlayerTemplateAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
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
			if (CMovement->IsMovingOnGround() && mMoveSpeed >= 50.f)
			{
				//�ִϸ��̼� �÷��׸� MoveXY �� ����
				//mPlayerAnim = EPlayerAnim::MoveXY;
				mIsFalling = false;
				if (bDebugEnabled)
					UE_LOG(TestGame, Display, TEXT("Character is Moving. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
			//�̵����� �ƴϰ� Falling ���϶� ó��
			else if (CMovement->IsFalling())
			{
				//�ִϸ��̼� �÷��׸� Falling ���� ó���Ѵ�
				mIsFalling = true;
				//mPlayerAnim = EPlayerAnim::Falling;
				if (bDebugEnabled)
					UE_LOG(TestGame, Display, TEXT("Character is Falling. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
			//�ٴڿ� �ְ� �̵��� ���� �� 
			else if (CMovement->IsMovingOnGround() && mMoveSpeed < 50.f)
			{
				mIsFalling = false;
				//mPlayerAnim = EPlayerAnim::Ground;
				if (bDebugEnabled)
					UE_LOG(TestGame, Display, TEXT("Character is on ground. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
		}
	}
}

void UPlayerTemplateAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//������Ÿ�� ���� ��������Ʈ ���ε�
	if (mLandMontage)
	{
		//��Ÿ�ֿ��� �� ��������Ʈ �Լ�
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUFunction(this, FName("OnLandMontageEnd"));
		Montage_SetEndDelegate(MontageEndDelegate, mLandMontage);

		//��Ÿ�� ���̵���� �� ��������Ʈ �Լ�
		FOnMontageBlendingOutStarted MontageBlendOutDelegate;
		MontageBlendOutDelegate.BindUFunction(this, FName("OnLandBlendOut"));
		Montage_SetBlendingOutDelegate(MontageBlendOutDelegate, mLandMontage);
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

void UPlayerTemplateAnimInstance::PostInitProperties()
{
	Super::PostInitProperties();

	// ������ �̸��� ���� ��� ������ ���̺�κ��� �ִϸ��̼� ������ ���´�.
	if (!mAnimName.IsNone())
	{
		UPlayerTemplateAnimInstance* CDO =
			Cast<UPlayerTemplateAnimInstance>(GetClass()->GetDefaultObject());

		if (CDO)
		{
			CDO->SetAnimData(mAnimName);
		}
	}

	//APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());

	//if(IsValid(PC))

	//	mCharacterState = PC->GetCharacterStatePtr();
}

void UPlayerTemplateAnimInstance::AnimNotify_AttackStart()
{
	if (bDebugEnabled)
	UE_LOG(TestGame, Display, TEXT("Notify AttackStart Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());
	
	if (IsValid(PC))
	{
		//�������̺�Ʈ�� �������� ���� ������Ȱ��ȭ��
		if(PC->bNormalAttackOverlap)
		{
			TArray<UBoxComponent*> mWeaponColBox = PC->GetWeaponColBox();
			if (mWeaponColBox.Num() > 0)
			{
				int32	Count = mWeaponColBox.Num();
				for (int32 i = 0; i < Count; i++)
				{
					UE_LOG(TestGame, Warning, TEXT("Weapon[%d] OverlapEvent Enabled"), i);
					//mWeaponColBox[i]->SetGenerateOverlapEvents(true);
					mWeaponColBox[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
					//mWeaponColBox[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				}
			}
		}
		else
		{
			//�������� �ƴϸ� cone ��� ��
			PC->NormalAttack();
		}
	}
}

void UPlayerTemplateAnimInstance::AnimNotify_AttackEnd()
{
	if (bDebugEnabled)
	UE_LOG(TestGame, Display, TEXT("Notify AttackEnd Called"));
	APlayerCharacter* PC = Cast<APlayerCharacter>(TryGetPawnOwner());

	if (IsValid(PC))
	{
		//������ ��� ����϶��� ���� �ݸ����� �����ϰ��Ѵ�
		if (PC->bNormalAttackOverlap)
		{
			TArray<UBoxComponent*> mWeaponColBox = PC->GetWeaponColBox();
			if (mWeaponColBox.Num() > 0)
			{
				int32	Count = mWeaponColBox.Num();
				for (int32 i = 0; i < Count; i++)
				{
					UE_LOG(TestGame, Warning, TEXT("Weapon[%d] OverlapEvent Disabled"), i);
					//mWeaponColBox[i]->SetGenerateOverlapEvents(false);
					mWeaponColBox[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				}
			}
		}
	}
}

void UPlayerTemplateAnimInstance::AnimNotify_ComboStart()
{
	if (bDebugEnabled)
	UE_LOG(TestGame, Display, TEXT("Notify ComboStart Called"));

	//�÷��� �޺� ON 
	SetCharacterState(FCharacterState::COMBO);
	//�÷��� ���� OFF -> �������� ���ݵ��� ACTING ���� ������ ��������
	ClearCharacterState(FCharacterState::ACTING);
}

void UPlayerTemplateAnimInstance::AnimNotify_ComboEnd()
{
	if (bDebugEnabled)
	UE_LOG(TestGame, Display, TEXT("Notify ComboEnd Called"));
	//�÷��� �޺� OFF 
	ClearCharacterState(FCharacterState::COMBO);
	//���� OFF -> ���� ���� ����
	//ClearCharacterState(FCharacterState::ACTING);
}

void UPlayerTemplateAnimInstance::AnimNotify_LandRecoveryEnd()
{
	if(bDebugEnabled)
		UE_LOG(TestGame, Display, TEXT("Notify LandEnd Called"));

	//���� ��� �Ϸ� : LANDING �� �����Ѵ�
	//�ٸ� �ִϸ��̼��� �̰����� �����ϱ� ���� ���� �� �ֱ� ������ 
	//EndDelegate �Լ��� �ϳ� �� ���ε��صд�
	//OnLandMontageEnd
	ClearCharacterState(FCharacterState::LANDING);
	ClearCharacterState(FCharacterState::ACTING);
}

void UPlayerTemplateAnimInstance::AnimNotify_DashEnd()
{
	//mDashAlpha = 0.f;
	//ClearCharacterState(FCharacterState::ACTING);
	//mPlayerAnim = EPlayerAnim::Ground;
	//UE_LOG(TestGame, Warning, TEXT("AnimNotify_DashEnd has been called"));
}

void UPlayerTemplateAnimInstance::AnimNotify_SpecialEnd()
{
	/*ClearCharacterState(FCharacterState::ACTING);
	mPlayerAnim = EPlayerAnim::Ground;
	UE_LOG(TestGame, Warning, TEXT("AnimNotify_SpecialEnd has been called"));*/
}

void UPlayerTemplateAnimInstance::OnLandMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	//��������Ʈ ���ε��� �Լ��̴�
	//���� ����� �ǵ����� ������ ������ (�ַ� �ִԱ׷����� ���� ���⼺�� ����)

	//������������ �̵��Ұ� ���¸� �����Ѵ�
	ClearCharacterState(FCharacterState::LANDING);
	ClearCharacterState(FCharacterState::ACTING);
	if (bDebugEnabled)
	UE_LOG(TestGame, Warning, TEXT("OnLandMontageEnd Called"));
}

void UPlayerTemplateAnimInstance::OnLandBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	//��������Ʈ ���ε��� �Լ��̴�
	//���� �ƿ��� ���� �̵��Ұ� ���¸� �������ش�
	//�������� ���� �̵��Ұ� ���¸� �����Ѵ�
	ClearCharacterState(FCharacterState::LANDING);
	ClearCharacterState(FCharacterState::ACTING);
	if (bDebugEnabled)
	UE_LOG(TestGame, Warning, TEXT("OnLandBlendOut Called"));
}

void UPlayerTemplateAnimInstance::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	//��������Ʈ ���ε��� �Լ��̴�
	//���� ��Ÿ���� ����� ���� �� �� ȣ��

	//�޺� ���� ���� 
	//ClearCharacterState(FCharacterState::COMBO);
	//���� �� ����
	ClearCharacterState(FCharacterState::ACTING);
	if (bDebugEnabled)
	UE_LOG(TestGame, Warning, TEXT("OnAttackMontageEnd has been called"));
}

void UPlayerTemplateAnimInstance::OnAttackMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	//��������Ʈ ���ε��� �Լ��̴�
	//���� ��Ÿ�ְ� ���� �ƿ� �� �� ȣ��
	
	//�޺� ���� ���� 
	//State->ClearState(State->COMBO);
	//������ ����
	ClearCharacterState(FCharacterState::ACTING);
	if (bDebugEnabled)
	UE_LOG(TestGame, Warning, TEXT("OnAttackMontageBlendOut has been called"));
}

