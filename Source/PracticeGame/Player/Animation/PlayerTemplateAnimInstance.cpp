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
	//Name 데이터행 하나를 읽어온다
	const		FPlayerAnimData* AnimData =
		mPlayerAnimDataTable->FindRow<FPlayerAnimData>(Name, TEXT(""));

	//읽어오면  멤버에 받아둔다
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
	//	// 안전하게 리턴 또는 대체 처리
	//	return;
	//}
	//mAnimMap 에서 시퀀스를 찾고 성공하면 해당 시퀀스의 재생시간을 가지고 타이머를 설정한다
	//타이머는 일정시간 이후 플래그를 되돌려서 다시 행동을 자유롭게 할 수 있게 해준다
	if (TObjectPtr<UAnimSequence>* FoundSequence = mAnimMap.Find(KeyName))
	{
		if (*FoundSequence)
		{
			UWorld* World = (GetOwningComponent() ? GetOwningComponent()->GetWorld() : nullptr);
			if (World)
			{
				//시퀀스의 재생시간을 얻어온다
				float AnimDuration = (*FoundSequence)->GetPlayLength();
				//재생시간 이후 각종 플래그 해제 함수를 동작시킨다
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
			//시퀀스가 없으면 플래그들을 즉시 해제한다
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
			//	// 안전하게 리턴 또는 대체 처리
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
		//공중 공격 연사 시 걸릴 수 있는 랜딩 무한결속됨 현상의 임시 해결 (발생 가능성 낮음)
		FCharacterState* StatePtr = PC->GetCharacterStatePtr();
		ClearCharacterState(FCharacterState::LANDING);
		//StatePtr->ClearState(StatePtr->LANDING);


		//현재 애님인스턴스에서 공격몽타주를 받아오고 성공하면 실행
		//함수부분을 플레이어에서 AnimInst 로 이동했기 때문에 자손을 그냥 불러오면 된다.
		/*UAnimMontage* AttackMontage = GetAttakMontage();*/
		if (mAttackMontage)
		{
			//플래그를 설정한다 ACTING : ON  -- 꺼질때까지 다음 행위를 못한다
			SetCharacterState(FCharacterState::ACTING);
				//StatePtr->SetState(StatePtr->ACTING);

			//콤보 상태인 경우 인덱스 순환 처리
			if (HasCharacterState(FCharacterState::COMBO))
			{
				//다음 몽타주 섹션의 인덱스로 순환 변경한다
				mSectionIndex
					= (mSectionIndex + 1) % mAttackSectionArray.Num();
				//다음 재생할 몽타주 이름할당(디버그 출력용)
				//FName NextSectionName = mAttackSectionArray[mSectionIndex];
			}
			//콤보가 아닌경우 인덱스 초기화
			else
			{
				mSectionIndex = 0;
			}

			//현재 인덱스를 활용하여 재생하려는 공격섹션의 이름을 받아온다
			FName SectionName = mAttackSectionArray[mSectionIndex];

			//커스텀함수: 공격 몽타주에서 
			//현재 할당된 섹션의 이름을 사용하여 그 섹션을 바로 재생시킨다 
			PlayMontageSection(mAttackMontage, SectionName);

			//디버깅출력 현재 재생 몽타주 이름과 인덱스 
			if(bDebugEnabled)
			UE_LOG(TestGame, Display,
				TEXT("Current Playing Montage [%s],\tIndex[%d]"),
				*SectionName.ToString(),
				mSectionIndex);

			//현재 액터의 위치와 회전값에 맞추기 위해 값을 가져옴
			FVector SpawnLocation = PC->GetActorLocation() + PC->GetActorForwardVector() * 100.0f;
			FRotator SpawnRotation = PC->GetActorRotation();

			//AATestProjectile 클래스로 투사체를 생성함
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
	//플래그 설정
	mDashAlpha = 1.0f;
	//아래 플래그가 할당되면 대시의 캐시된 스테이트머신이 선택된다
	mPlayerAnim = EPlayerAnim::Dash;
	//모션이 항상 출력될 수 있도록 플레이백 포지션을 초기화한다
	

	//애니메이션 시간 이후 플래그를 끄도록 타이머를 설정한다
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
			//스페샬 사용상태와 관련된 플래그들을 설정한다
			mPlayerAnim = EPlayerAnim::Special;

			//타이머를 설정한다
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
			//Cleave 사용상태와 관련된 플래그들을 설정한다
			mPlayerAnim = EPlayerAnim::Cleave;

			//타이머를 설정한다
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
	//몽타주를 재생한다
	Montage_Play(Montage);
	//재생하는 몽타주의 섹션을 점프시킨다
	Montage_JumpToSection(SectionName, Montage);
}

void UPlayerTemplateAnimInstance::ResetPlayerAnimSeq(const FName AnimName)
{
	//유효성 검사
	//안전하게 처리하기 위해 유효한지 검사한다
	if (mAnimMap.Contains(AnimName))
	{
		UAnimSequence* AnimSeq = mAnimMap[AnimName];
		if (AnimSeq)
		{
			//현재 애님 시퀀스 플레이어를 찾아서 리셋한다
			StopAllMontages(0.0f);		//몽타주들을 재생 중단시킨다
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
			if (CMovement->IsMovingOnGround() && mMoveSpeed >= 50.f)
			{
				//애니메이션 플래그를 MoveXY 로 설정
				//mPlayerAnim = EPlayerAnim::MoveXY;
				mIsFalling = false;
				if (bDebugEnabled)
					UE_LOG(TestGame, Display, TEXT("Character is Moving. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
			//이동중이 아니고 Falling 중일때 처리
			else if (CMovement->IsFalling())
			{
				//애니메이션 플래그를 Falling 으로 처리한다
				mIsFalling = true;
				//mPlayerAnim = EPlayerAnim::Falling;
				if (bDebugEnabled)
					UE_LOG(TestGame, Display, TEXT("Character is Falling. mMoveSpeed [%0.0f]"), mMoveSpeed);
			}
			//바닥에 있고 이동이 없을 때 
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
	//착지몽타주 관련 델리게이트 바인딩
	if (mLandMontage)
	{
		//몽타주엔드 시 델리게이트 함수
		FOnMontageEnded MontageEndDelegate;
		MontageEndDelegate.BindUFunction(this, FName("OnLandMontageEnd"));
		Montage_SetEndDelegate(MontageEndDelegate, mLandMontage);

		//몽타주 페이드오프 시 델리게이트 함수
		FOnMontageBlendingOutStarted MontageBlendOutDelegate;
		MontageBlendOutDelegate.BindUFunction(this, FName("OnLandBlendOut"));
		Montage_SetBlendingOutDelegate(MontageBlendOutDelegate, mLandMontage);
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

void UPlayerTemplateAnimInstance::PostInitProperties()
{
	Super::PostInitProperties();

	// 지정된 이름이 있을 경우 데이터 테이블로부터 애니메이션 정보를 얻어온다.
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
		//오버렙이벤트로 설정했을 때만 오버렙활성화함
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
			//오버렙이 아니면 cone 계산 함
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
		//오버렙 계산 모드일때만 무기 콜리전을 설정하게한다
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

	//플래그 콤보 ON 
	SetCharacterState(FCharacterState::COMBO);
	//플래그 액팅 OFF -> 이제부터 공격등의 ACTING 유형 행위가 가능해짐
	ClearCharacterState(FCharacterState::ACTING);
}

void UPlayerTemplateAnimInstance::AnimNotify_ComboEnd()
{
	if (bDebugEnabled)
	UE_LOG(TestGame, Display, TEXT("Notify ComboEnd Called"));
	//플래그 콤보 OFF 
	ClearCharacterState(FCharacterState::COMBO);
	//액팅 OFF -> 다음 공격 가능
	//ClearCharacterState(FCharacterState::ACTING);
}

void UPlayerTemplateAnimInstance::AnimNotify_LandRecoveryEnd()
{
	if(bDebugEnabled)
		UE_LOG(TestGame, Display, TEXT("Notify LandEnd Called"));

	//착지 모션 완료 : LANDING 를 해제한다
	//다른 애니메이션이 이과정에 도달하기 전에 덮을 수 있기 떄문에 
	//EndDelegate 함수를 하나 더 바인딩해둔다
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
	//델리게이트 바인딩된 함수이다
	//착지 모션이 의도보다 빠르게 끝나도 (주로 애님그래프의 조건 복잡성에 의해)

	//렌딩으로인한 이동불가 상태를 해제한다
	ClearCharacterState(FCharacterState::LANDING);
	ClearCharacterState(FCharacterState::ACTING);
	if (bDebugEnabled)
	UE_LOG(TestGame, Warning, TEXT("OnLandMontageEnd Called"));
}

void UPlayerTemplateAnimInstance::OnLandBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	//델리게이트 바인딩된 함수이다
	//블렌드 아웃될 때도 이동불가 상태를 해제해준다
	//렌딩으로 인한 이동불가 상태를 해제한다
	ClearCharacterState(FCharacterState::LANDING);
	ClearCharacterState(FCharacterState::ACTING);
	if (bDebugEnabled)
	UE_LOG(TestGame, Warning, TEXT("OnLandBlendOut Called"));
}

void UPlayerTemplateAnimInstance::OnAttackMontageEnd(UAnimMontage* Montage, bool bInterrupted)
{
	//델리게이트 바인딩된 함수이다
	//공격 몽타주의 재생이 종료 될 때 호출

	//콤보 가능 해제 
	//ClearCharacterState(FCharacterState::COMBO);
	//액팅 중 해제
	ClearCharacterState(FCharacterState::ACTING);
	if (bDebugEnabled)
	UE_LOG(TestGame, Warning, TEXT("OnAttackMontageEnd has been called"));
}

void UPlayerTemplateAnimInstance::OnAttackMontageBlendOut(UAnimMontage* Montage, bool bInterrupted)
{
	//델리게이트 바인딩된 함수이다
	//공격 몽타주가 블렌드 아웃 될 때 호출
	
	//콤보 가능 해제 
	//State->ClearState(State->COMBO);
	//액팅중 해제
	ClearCharacterState(FCharacterState::ACTING);
	if (bDebugEnabled)
	UE_LOG(TestGame, Warning, TEXT("OnAttackMontageBlendOut has been called"));
}

