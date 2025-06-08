// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Player/Animation/AnimInstancePlayer.h"
//로그 카테고리 설정
DEFINE_LOG_CATEGORY(PlayerLog);

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//인스턴스를 안전하게 로드한다. 
	// TEXT("경로") 는 원하는 에셋의 경로를 참조한다
	//SkeletalMesh 객체에 로드했다. 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> 
		SkeletalMesh(TEXT(
			"/Script/Engine.SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));
	// 인스턴스 로드에 성공하면 캐릭터 클래스의 메쉬컴포넌트를
	// 로드된 메쉬로 설정한다.
	if (SkeletalMesh.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(SkeletalMesh.Object);

	//스켈레탈메시 Z = -90.f
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.5f));
	//스켈레탈메시의 Yaw = -90.f
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	//캡슐 절반 높이 = 90.5
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.5f);

	//플레이어 Yaw 180.f
	SetActorRelativeRotation(FRotator(0, 180, 0));
	

	/*
		서브오브젝트 : 컴포넌트의 추가
		스프링암과 카메라 컴포넌트를 추가한다
	*/
	mSpringArm = CreateDefaultSubobject
		<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject
		<UCameraComponent>(TEXT("Camera"));

	/*
		스프링암은 루트 컴포넌트를 부모로 한다
		카메라는 스프링암을 부모로 한다
	*/

	mSpringArm->SetupAttachment(RootComponent);
	mCamera->SetupAttachment(mSpringArm);

	/* 
		스프링암의 타깃 암 길이를 300.f 로 설정한다
		스프링암의 Pitch  를 -20 만큼 상대적으로 회전한다
		스프링암의 Z 벡터위치를 +39.0f 만큼 상대적으로 이동한다
		*/
	mSpringArm->TargetArmLength = 300.f;
	mSpringArm->SetRelativeRotation(FRotator(-20, 0, 0));
	mSpringArm->SetRelativeLocation(FVector(0, 0, 39));

	// mCamera->AttachToComponent(mSpringArm); 
	// AttachToComponent 는 해당 컴포넌트를 USceneComponent 에 부착시킨다
	
		/*
	1. UClass 포인터 가져오기
		: APlayerCharacter 의 UClass 정보를 가져온다
				UClass 는 리플렉션 시스템을 통해 해당 클래스의 
				기본 정보를 저장하는 구조체이다 */
	//UClass* MyClass = APlayerCharacter::StaticClass();
	// 이제 MyClass 는 APlayerCharacter 클래스의 UClass 정보를 참조한다

	/*
	2. CDO 가져오기
		: GetDafaultObject<ACharacter>()  로   Class Default Object 를 반환한다
		반환된 CDO 는 이 클래스의 기본 인스턴스이다.
		ACharacter 클래스의 모든 인스턴스가 이 객체를 기반으로 생성된다는 의미이다
	*/
	//ACharacter* DefaultCharacter = MyClass->GetDefaultObject();
	//이제 DefaultCharacter 는  CPlayerCharacter 클래스의 기본 객체가 된다.

	//캐릭터무브먼트 컴포넌트를 얻어온다
	//GetCharacterMovement 는 
	// UCharacterMovementComponent* 를 반환한다
	
	//JumpZVelocity 점프 높이를 설정한다
	GetCharacterMovement()
		->JumpZVelocity = 600.f;	
	//GravityScale  중력을 설정한다
	GetCharacterMovement()
		->GravityScale = 1.0f; 
	//AirControl  공중 이동 제어를 설정한다, 
	// 낮을 수록 이동이 제한되고, 높을수록 공중 이동에 자유로워진다 
	GetCharacterMovement()
		->AirControl = 0.5f;

	//JumpMaxCount 는  착지하기 전까지 최대 가능한 점프 횟수를 결정한다
	JumpMaxCount = 2;

	/*
		애니메이션 블루프린트 에셋 로드
		애님인스턴스를 상속받고 있는 TSubClassOf<UAnimInstance> 타입의 클래스이다
		FClassFinder 를 사용해야 하며, 
		블루프린트이기 때문에 경로 마지막에 _C'  를 붙여야 한다 	*/
	static ConstructorHelpers::FClassFinder<UAnimInstance> 
		ABPCharacter(
			TEXT(
				"/Script/Engine.AnimBlueprint'/Game/Player/Animation/ABP/ABPCharacter.ABPCharacter_C'")
		);

	//에셋 로드에 성공하면  현재 스켈레탈 메쉬의  애님인스턴스 클래스로 설정한다
	if (ABPCharacter.Succeeded())
		GetMesh()->SetAnimInstanceClass(ABPCharacter.Class);

	//투사체 에셋로드
	static ConstructorHelpers::FClassFinder<AATestProjectile> TestProjectile(
		TEXT(
			"/Script/CoreUObject.Class'/Script/PracticeGame.ATestProjectile'")
	);
	//에셋 주소 할당
	if (TestProjectile.Succeeded())
		mProjectileClass = TestProjectile.Class;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//빙의된 플레이어 컨트롤러를 얻어온다
	//GetController 는 AController 타입을 반환하기 때문에 캐스팅이 필요하다
	//UObject 를 상속받고 있어 Cast 를 써서 안전하게 형변환을 할 수 있다

	APlayerController* PlayerController = 
		Cast<APlayerController>(GetController());

	//캐스팅에성공하면
	if (IsValid(PlayerController))
	{
		//현재 플레이어의 로컬 서브시스템을 가져온다 
		//즉, 현재 플레이어가 사용하고 있는 향상된 입력 처리 서브시스템을 가져온다
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::
			GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(PlayerController->GetLocalPlayer());

		//CDO 에서 APlayerControllerTestGame 을 가져온다
		const APlayerControllerTestGame* APCTestGameCDO = GetDefault<APlayerControllerTestGame>();

		//매핑 컨텍스트를 현재 플레이어의 서브시스템에 추가한다
		Subsystem->AddMappingContext(APCTestGameCDO->mIMC, 0);
	}

	//애님인스턴스를 받아온다
	mAnimInst = Cast<UAnimInstancePlayer>(GetMesh()->GetAnimInstance());
}

/*BindAction 으로 바인딩 되어 있으며,
입력액션 'IAMove' 가 Triggered 될 때 호출되는 함수이다.*/
void APlayerCharacter::OnIAMoveTriggered(const FInputActionValue& Value)
{
	//이동 가능할 때만 실행
	if (!IsCharacterImmobilized())
	{
		/*
		인자로 받아오는 값은 입력 액션 설정과 매핑 컨텍스트에서 정의한 내용을 따른다
			정의
				IA_Move 는 2D 형식으로 정의하고 있다
				매핑컨텍스트에서는
					키보드 입력을 X 축과 Y 축(스위즐 된 경우) 으로 매핑하였다
				Triggered 라면 1 (부정 인 경우 -1)
							 아니면 0 이다*/

							 //입력값을 벡터로 변환한 값을 저장할 변수를 선언한다
							 //벡터 계산을 위해 쓰일 것이다.
		FVector Axis;

		//입력에서 전달된 값을 벡터로 형변한다.
		//Value 값의 예외 처리를 위해 switch 문의 형태로 구현했다
		switch (Value.GetValueType())
		{
		case EInputActionValueType::Axis1D:
			Axis.X = FMath::Clamp(Value.Get<float>(), -1.0f, 1.0f);
			if (bDebugEnabled)
				UE_LOG(PlayerLog, Display,
					TEXT("InputActionType [Axis1D],\t %.1f"),
					Axis.X);
			break;
		case EInputActionValueType::Axis2D:
		{
			FVector2D Axis2D = Value.Get<FVector2D>();
			Axis.X = FMath::Clamp(Axis2D.X, -1.0f, 1.0f);
			Axis.Y = FMath::Clamp(Axis2D.Y, -1.0f, 1.0f);
			if (bDebugEnabled)
				UE_LOG(PlayerLog, Display,
					TEXT("InputActionType [Axis2D],\t( %.1f, %.1f )"),
					Axis.X, Axis.Y);
			break;
		}
		case EInputActionValueType::Axis3D:
			Axis = Value.Get<FVector>();
			if (bDebugEnabled)
				UE_LOG(PlayerLog, Display,
					TEXT(" InputActionType [Axis3D],\t( %.1f, %.1f, %.1f )"),
					Axis.X, Axis.Y, Axis.Z);
			break;
		default:
			if (bDebugEnabled)
				UE_LOG(LogTemp, Warning,
					TEXT("Unexpected InputActionValueType received: %d"),
					static_cast<int32>(Value.GetValueType()));
			return;
		}

		// 방향을 계산하기위한 FVector 객체이다. 0으로 초기화한다
		FVector Direction = FVector::Zero();

		//X 축의 값을 받아 현재엑터의 전방벡터 (X축) 에 곱한다
		//Y 축의 값을 받아 현재 엑터의 우측벡터 (Y축) 에 곱하여 모든 값을 저장한다
		Direction = GetActorForwardVector() * Axis.X;
		Direction += GetActorRightVector() * Axis.Y;

		/* 또는
		 Direction = FVector(GetActorForwardVector() * Axis.X,
										GetActorRightVector() * Axis.Y,
										GetActorUpVector());							*/

										//정규화를 하여 대각선의 입력을 매끄럽게 처리한다.
										//예외 처리 0 정규화를 방지한다
		if (!Direction.IsNearlyZero())
		{
			//예외 처리 부동소수점 연산오류(아주 작은 값 등) 를 방지한다
			const float Threshold = KINDA_SMALL_NUMBER;
			if (Direction.SizeSquared() > Threshold)
			{
				Direction.Normalize();
				//예외 처리 정규화 후 크기 보정
				Direction *= 1.0f / FMath::Max(Direction.Size(), 1.0f);
				if (bDebugEnabled)
					UE_LOG(PlayerLog, Display, TEXT("Direction Normalized Safety : ( %.1f, %.1f, %.1f )"), Direction.X, Direction.Y, Direction.Z);
			}
		}

		//현재 플레이어 메쉬의 애님인스턴스를 받아 
		// UAnimInstancePlayer 로 캐스팅한다
		UAnimInstancePlayer* AnimInst =
			Cast<UAnimInstancePlayer>(GetMesh()->GetAnimInstance());

		//캐스팅에 성공하면 
		//Axis 를
		//		: 입력으로 받은 X 및 Y 축 을  FVector 로 변환 한 값
		//애님인스턴스의 mDirection 멤버에 할당한다
		if (IsValid(AnimInst))
			AnimInst->SetDirection(Axis);

		//계산 완료된 벡터로 아래의 함수를 호출한다
		//MoveToAxis 는 이동을 처리하며 이동과 관련된 추가 기능을 구현할 수 있도록 
		//사용자 정의로 작성한 멤버 함수이다
		float ScaleValue = FMath::Max(
			FMath::Abs(Axis.X),
			FMath::Abs(Axis.Y)
		);

		MoveToAxis(Direction, ScaleValue);
	}
	else
	{
		if (bDebugEnabled)
			UE_LOG(PlayerLog, Display, TEXT("Cannot Move Character : Immobillized"));
	}
}

//이 함수는 점프 함수로 
// IA_Jump 로 매핑한 키가 Started 되었을때 호출된다.
void APlayerCharacter::OnIAJumpStarted(const FInputActionValue& Value)
{
	//CanJump 는 점프가 가능한 상태인지를 반환한다
	//점프가 가능하고 이동불가가 아닐 때만 실행한다
	if (CanJump() && !IsCharacterImmobilized())
	{
		GetCharacterMovement()->bNotifyApex = true; // 최고점 알림 활성화
		//mCharacterState.ClearState(mCharacterState.ACTING);
		//Jump 는 캐릭터가 점프 하도록 하는 멤버 함수이다.
		Jump();

		if (bDebugEnabled)
			UE_LOG(PlayerLog, Display, TEXT("Character : Jump Called"));

		if (mAnimInst)
		{
			mAnimInst->SetPlayerAnim(EPlayerAnim::Jump);
			mAnimInst->SetIsJumping(true);
		}
	}
}

void APlayerCharacter::OnIAAttackStarted(const FInputActionValue& Value)
{
	//행위가 가능할 때만 공격을 한다 예외. 스턴/다른액팅/높은데서의착지모션 등 
	if (CanCharacterAct())
	{
		//공중 공격 연사 시 걸릴 수 있는 랜딩 무한결속됨 현상의 임시 해결 (발생 가능성 낮음)
		mCharacterState.ClearState(mCharacterState.LANDING);

		if (mAnimInst)
		{
			//현재 들고 있는 애님인스턴스에서 공격몽타주를 받아오고 성공하면 실행
			UAnimMontage* AttackMontage = mAnimInst->GetAttakMontage();
			if (AttackMontage)
			{
				//플래그를 설정한다 ACTING : ON  -- 꺼질때까지 다음 행위를 못한다
				mCharacterState.SetState(mCharacterState.ACTING);

				//콤보 상태인 경우 인덱스 순환 처리
				if (mCharacterState.HasState(mCharacterState.COMBO))
				{
					//다음 몽타주 섹션의 인덱스로 순환 변경한다
					mAnimInst->mSectionIndex 
						= (mAnimInst->mSectionIndex + 1) % mAnimInst->mAttackSectionArray.Num();
					//다음 재생할 몽타주 이름할당(디버그 출력용)
					//FName NextSectionName = mAnimInst->mAttackSectionArray[mAnimInst->mSectionIndex];
				}
				//콤보가 아닌경우 인덱스 초기화
				else
				{
					mAnimInst->mSectionIndex = 0;
				}

				//현재 인덱스를 활용하여 재생하려는 공격섹션의 이름을 받아온다
				FName SectionName = mAnimInst->mAttackSectionArray[mAnimInst->mSectionIndex];
				
				//커스텀함수: 공격 몽타주에서 
				//현재 할당된 섹션의 이름을 사용하여 그 섹션을 바로 재생시킨다 
				mAnimInst->PlayMontageSection(AttackMontage, SectionName);
				
				//디버깅출력 현재 재생 몽타주 이름과 인덱스 
				UE_LOG(TestGame, Display,
					TEXT("Current Playing Montage [%s],\tIndex[%d]"),
					*SectionName.ToString(),
					mAnimInst->mSectionIndex);

				//현재 액터의 위치와 회전값에 맞추기 위해 값을 가져옴
				FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
				FRotator SpawnRotation = GetActorRotation();

				//AATestProjectile 클래스로 투사체를 생성함
				AATestProjectile* Projectile = GetWorld()->SpawnActor<AATestProjectile>(mProjectileClass, SpawnLocation, SpawnRotation);
				if (bDebugEnabled)
					UE_LOG(PlayerLog, Display, TEXT("Character : Attack"));
			}
			else
				UE_LOG(TestGame, Warning, TEXT("No Montage for this action exists"));
		}
		else
		{
			UE_LOG(TestGame, Warning, TEXT("No AnimInst for this class exists"));
		}
	}
}

void APlayerCharacter::OnIAAttackTriggered(const FInputActionValue& Value)
{
	OnIAAttackStarted(Value);
}

void APlayerCharacter::OnIACamRotateTriggered(const FInputActionValue& Value)
{		
	//회전 값 FRotator 계산
	//입력값에서 X 및 Y 값을 가져온다
	FVector2D Input = Value.Get<FVector2D>();
		/* 입력값 상세 정보
			1. 마우스 이동하기 (가속도에 가변하는 실수)
				좌측이동   -X 
				우측이동	+X 
				상방이동	-Y
				하방이동	+Y
			2. 게임패드 우측스틱 기울이기 ( 0 ~ 1  실수,  음수의 경우 -1 ~ 0)
				좌측으로기울이기		-X
				우측으로기울이기		+X
				위로울이기			+Y
				아래로기울이기		-Y				*/

	//회전속도의 배율 정의한다
	float RotationSpeed = 2.0f;
	//세로회전각의 한계를 정의한다
	float MaxPitchLimit = 75.0f;
	//축뒤틀림 방지용: 순간적으로 90도를 초과하는 너무 큰 값 처리
	Input.Y = (85 - MaxPitchLimit) < RotationSpeed * Input.Y 
		? (85 - MaxPitchLimit) / RotationSpeed 
		: Input.Y;
	Input.Y = (MaxPitchLimit - 85) > RotationSpeed * Input.Y
		? (MaxPitchLimit - 85) / RotationSpeed
		: Input.Y;

	//스프링암 존재를 체크하고나서
	//입력받은 두 축값(float)을 
	// 스프링암의 상대 회전의 각 요소에 가산한다
	// ** 트랜스폼 창 순서는  Roll, Yaw, Pitch
	
	if (mSpringArm)
	{
		//오일러 회전
		//// 현재 스프링암의 회전값을 가져온다
		//FRotator NewRotation = mSpringArm->GetRelativeRotation();
		//
		//// 위아래 입력시   Pitch 에 가산 +,  아래로 입력시  -
		//NewRotation.Pitch += Input.Y * RotationSpeed; 
		//// 좌우 입력 시 Yaw에 가산, 좌- 우+
		//NewRotation.Yaw += Input.X * RotationSpeed;
		//
		//***오일러 회전의 문제 완화 
		//1. Pitch 제한 적용
		//if (FMath::Abs(NewRotation.Pitch) >= 85.0f)
		//	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -85.0f, 85.0f);
		////회전 값을 적용한다
		//mSpringArm->SetRelativeRotation(NewRotation);

		//2. 쿼터니언 활용
		FQuat CurrentQuat = mSpringArm->GetRelativeRotation().Quaternion();

		// 입력값을 기반으로 회전 변환 생성 (Yaw 먼저, Pitch 나중)
		FQuat PitchQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(-Input.Y * RotationSpeed));
		FQuat YawQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(Input.X * RotationSpeed));
	
		// 새로운 회전 적용
		//FQuat NewQuat = YawQuat * CurrentQuat * PitchQuat;    //기본 방법
		// Pitch 제한 방식 : Yaw 를 제외해둔다
		FQuat NewQuat = CurrentQuat * PitchQuat;	
		
		// PitchQuat * YawQuat * CurrentQuat;
		// 회전 순서는 Pitch 90 지점에서  Y 와 Z 가 뒤틀리는문제를 해결한다. 
		// 하지만 해당지점에 도달하면  Y 회전하면서 피치가 역방향으로 되돌아가는 형태를 띈다
		//    FQuat NewQuat = PitchQuat * YawQuat * CurrentQuat;

		NewQuat.Normalize();

		// 현재 Pitch 값을 쿼터니언에서 직접 가져와 제한을 적용
		FVector ForwardVector = NewQuat.GetForwardVector();
		FRotator RotationFromQuat = ForwardVector.Rotation();
		//제한 적용 : 상하 카메라회전은 아래와 같이 제한한다
		RotationFromQuat.Pitch = FMath::Clamp(RotationFromQuat.Pitch, -MaxPitchLimit, MaxPitchLimit);

		// 제한된 Pitch를 반영하여 쿼터니언 생성 후 Yaw 를 적용
		FQuat ClampedQuat = FQuat::MakeFromRotator(RotationFromQuat);
		ClampedQuat.Normalize();

		FQuat FinalQuat = YawQuat * ClampedQuat;
		mSpringArm->SetRelativeRotation(FinalQuat);

		////순간 Pitch 변화량 제한 로직 델타의 한계만들기
		//// Pitch 에 변화량 제한을 적용한다
		//float PitchDelta = RotationFromQuat.Pitch - PrevPitch;
		//PitchDelta = FMath::Clamp(PitchDelta, -MaxPitchChange, MaxPitchChange);
		//RotationFromQuat.Pitch = PrevPitch + PitchDelta;

		//FQuat ClampedQuat = FQuat::MakeFromRotator(RotationFromQuat);
		//ClampedQuat.Normalize();

		//// 최종적으로 적용
		//mSpringArm->SetRelativeRotation(ClampedQuat);

		//// 이전 Pitch 값 갱신
		//PrevPitch = RotationFromQuat.Pitch;

		//3. Roll 값 보정
		//Roll 을 강제로 초기화하여 전달한다
		//mSpringArm->SetRelativeRotation(FRotator(NewRotation.Pitch, NewRotation.Yaw, 0));
		
		//애님인스턴스에도 이 정보를 건네준다
		// 짐벌락 해결방법 1. 로테이터 Y 클램프 일 때 전달방법
		//mAnimInst->SetCamRotateByInput(NewRotation);
		// 짐벌락 해결방법 2. 쿼터니언 회전  일 때 전달방법
		mAnimInst->SetCamRotateByInput(FinalQuat.Rotator());
		
		//디버깅 출력용
		if (bDebugEnabled)
		{
			FString OutputString;

			switch (Value.GetValueType())
			{
			case EInputActionValueType::Boolean:
				OutputString = FString::Printf(TEXT("Boolean: %s"), Value.Get<bool>() ? TEXT("true") : TEXT("false"));
				break;
			case EInputActionValueType::Axis1D:
				OutputString = FString::Printf(TEXT("Axis1D: %f"), Value.Get<float>());
				break;
			case EInputActionValueType::Axis2D:
			{
				FVector2D Vec = Value.Get<FVector2D>();
				OutputString = FString::Printf(TEXT("Axis2D: X=%f, Y=%f"), Vec.X, Vec.Y);
			}
			break;
			case EInputActionValueType::Axis3D:
			{
				FVector Vec = Value.Get<FVector>();
				OutputString = FString::Printf(TEXT("Axis3D: X=%f, Y=%f, Z=%f"), Vec.X, Vec.Y, Vec.Z);
			}
			break;
			default:
				OutputString = TEXT("Unknown Type");
				break;
			}

			//디버그
			UE_LOG(TestGame,
				Log,
				TEXT("Camera Rotate key triggered [%s],\t Rotate [%s]"), *OutputString, *FinalQuat.Rotator().ToString()	);
		}
	}
}

void APlayerCharacter::MoveToAxis(const FVector& Axis, const float ScaleValue)
{
	if (FMath::Abs(ScaleValue) > KINDA_SMALL_NUMBER)
		AddMovementInput(Axis, ScaleValue);
	else
		AddMovementInput(Axis);
}

bool APlayerCharacter::IsCharacterImmobilized()
{
	//하나라도 true 면 이동불능 상태이다
	if (mCharacterState.HasState(mCharacterState.STUNNED) ||
		mCharacterState.HasState(mCharacterState.IMMOBILIZED) ||
		mCharacterState.HasState(mCharacterState.LANDING)
		)
	{
		return true;
	}
	return false;
}

bool APlayerCharacter::CanCharacterAct()
{
		//하나라도 true 면 행동불능 상태이다
		if (mCharacterState.HasState(mCharacterState.ACTING)	||
			mCharacterState.HasState(mCharacterState.STUNNED) ||
			mCharacterState.HasState(mCharacterState.LANDING)
			)
		{
			return false;
		}
		return true;
}

FCharacterState* APlayerCharacter::GetCharacterState()
{
	return &mCharacterState;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//인자의 PlayerInputComponent 는 
	// APlayerController 클래스의 InputComponent* 이다
	// 이 컴포넌트를 향상된입력컴포넌트* 로 캐스팅한 객체를 생성한다.
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//캐스팅에 성공시
	if (IsValid(EnhancedInputComponent))
	{
		//CDO 에서 APlayerControllerTestGame 을 가져온다
		const APlayerControllerTestGame* APCTestGameCDO = GetDefault<APlayerControllerTestGame>();

		//IA_Move 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Triggered 형식으로 호출한다.
		//		--> 플레이어캐릭터의 멤버함수 OnIAMoveTriggered 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIAMove,
				ETriggerEvent::Triggered,
				this,
				&APlayerCharacter::OnIAMoveTriggered);

		//IA_Jump 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
		//		--> 플레이어캐릭터의 멤버함수 OnIAJumpStarted 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIAJump,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::OnIAJumpStarted);

		//IA_Attack 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
		//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIAAttack,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::OnIAAttackStarted);

		//IA_Attack 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Triggered 형식으로 호출한다.
		//		--> 플레이어캐릭터의 멤버함수 OnIAAttackTriggered 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIAAttack,
				ETriggerEvent::Triggered,
				this,
				&APlayerCharacter::OnIAAttackTriggered);

		//IA_CameraRotate 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Triggered 형식으로 호출한다.
		//		--> 플레이어캐릭터의 멤버함수 OnIACamRotateTriggered 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIACameraRotate,
				ETriggerEvent::Triggered,
				this,
				&APlayerCharacter::OnIACamRotateTriggered);
	}
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (mAnimInst)
	{
		if (mAnimInst->GetIsJumping())
		{
			mAnimInst->SetIsJumping(false);
			mAnimInst->SetPlayerAnim(EPlayerAnim::Landed);
		}

		//중력 절댓값
		float GravityAbs = FMath::Abs(GetCharacterMovement()->GetGravityZ());
		//이론상 최대 점프 높이를 계산한다
		float MaxJumpHeight 
			= (GetCharacterMovement()->JumpZVelocity * GetCharacterMovement()->JumpZVelocity)
			/ (2 * GravityAbs);
		//이론상 기본 점프시 착지 속도 (보정값 5%추가)
		float MaxLandingSpeed
			= sqrt(2 * GravityAbs * MaxJumpHeight) * 1.05f;
		//착지 속도를 가져온다 (X Y 이동 속도를 배제하기 위해 Z 축만 계산한다
		float CurrentVerticalSpeed
			= FMath::Abs(GetCharacterMovement()->GetLastUpdateVelocity().Z);


		//보통 점프가 아닌 착지라면 렌딩 몽타주 재생(노티파이까지 잠시동안 이동과 행동 불능 처리)
		if (CurrentVerticalSpeed > MaxLandingSpeed)
		{
			UE_LOG(PlayerLog, Display, TEXT("falling from higher apex than normal jump.\tSpeed %0.2f  > MAX[%0.2f]"), CurrentVerticalSpeed, MaxLandingSpeed);
			//케릭터 상태 플래그에 이동불가를 설정한다 (LandEnd 노티파이에서 해제한다)
			mCharacterState.SetState(mCharacterState.LANDING);
			//착지 중 공격 행위가 일으키는 공격 플래그 안풀림 문제 해결 
			mCharacterState.SetState(mCharacterState.ACTING);
			UAnimMontage* LandMontage = mAnimInst->GetLandMontage();
			//Land몽타주 재생
			mAnimInst->Montage_Play(LandMontage);
		}
		else
		{
			UE_LOG(PlayerLog, Display, TEXT("equal or less than normal jump.\tSpeed % 0.2f <= MAX[% 0.2f]"), CurrentVerticalSpeed, MaxLandingSpeed);
		}
		
		//if(bDebugEnabled)
		UE_LOG(PlayerLog, Display, TEXT("Player Landed"));
	}
}

void APlayerCharacter::Falling()
{
	Super::Falling();
}

void APlayerCharacter::NotifyJumpApex()
{
	Super::NotifyJumpApex();

	UE_LOG(PlayerLog, Display, TEXT("Player on Apex"));
}
