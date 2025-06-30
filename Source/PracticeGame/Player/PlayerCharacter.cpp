// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "PlayerData.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../NPC/Monster/Monster.h"
#include "../PlayerController/PlayerControllerTestGame.h"
#include "Animation/AnimInstancePlayer.h"
#include "Animation/PlayerTemplateAnimInstance.h"
#include "../UI/MainGame/PlayerStatusHUDWidget.h"
#include "../UI/Character/Panel/CharacterInfoWidget.h"
#include "../UI/WidgetManager.h"
#include "../Data/Item/ItemDataCacheManager.h"

//로그 카테고리 설정
DEFINE_LOG_CATEGORY(PlayerLog);

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	mInventoryItemsComponent = CreateDefaultSubobject
		<UInventoryComponent>(TEXT("InvItems"));
	mQuestComponent = CreateDefaultSubobject
		<UQuestComponent>(TEXT("PlayerQuestManager"));

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

	//대시 사운드 에셋
	static ConstructorHelpers::FObjectFinder<USoundWave> SfxDashAsset
	(TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_dash.sfx_dash'"));
	
	if (SfxDashAsset.Succeeded())
		mSfxDash = SfxDashAsset.Object;

	SetGenericTeamId(FGenericTeamId(TeamPlayer));

	bNormalAttackOverlap = false;

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Player"));

	//스텐실 설정
	GetMesh()->SetRenderCustomDepth(true);
	//GetMesh()->SetCustomDepthStencilValue(1);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//데이터 테이블로 부터 데이터 가져오기 각각의 클래스별로 생성자에서 mDataKey 를 정의하고 있기 떄문에 알맞게 세팅이 될것이다
	const UPlayerData* PlayerData =
		GetDefault<UPlayerData>();

	if (PlayerData)
	{
		FPlayerTableInfo* TableInfo =
			PlayerData->FindPlayerInfo(mDataKey);

		if (TableInfo)
		{
			mJob = TableInfo->Job;
			mName = TableInfo->Name;
			mAttack = TableInfo->Attack;
			mDefense = TableInfo->Defense;
			mHP = TableInfo->HP;
			mHPMax = TableInfo->HP;
			mMP = TableInfo->MP;
			mMPMax = TableInfo->MP;
			mAttackDistance = TableInfo->AttackDistance;
			mMoveSpeed = TableInfo->MoveSpeed;
			mLevel = TableInfo->Level;
			mExp = TableInfo->Exp;
			mGold = TableInfo->Gold;

			GetCharacterMovement()->MaxWalkSpeed = mMoveSpeed;

			//mMovement->SetMaxSpeed(mMoveSpeed);

			UE_LOG(TestGame, Warning, TEXT("Player Info"));
			//MainWidget 이 뷰포트에 뿌려진 다음 (컨트롤러의 BeginPlay) 이므로  값을 여기서 전달해도 좋다
			UPlayerStatusHUDWidget* HUDWidget = CWidgetManager::GetInst()->FindWidget<UPlayerStatusHUDWidget>(TEXT("PlayerStatusHUDWidget"));
			if (HUDWidget)
			{
				HUDWidget->SetPlayerName(mName);
				HUDWidget->SetHPBar(mHP, mHPMax);  
				HUDWidget->SetMPBar(mMP, mMPMax);
			}
		}

		FLevelExpRow* ExpInfo = PlayerData->FindExpInfo(FName(*FString::FromInt(mLevel+1)));

		if (ExpInfo)
		{
			//MainWidget 이 뷰포트에 뿌려진 다음 (컨트롤러의 BeginPlay) 이므로  값을 여기서 전달해도 좋다
			UPlayerStatusHUDWidget* HUDWidget = CWidgetManager::GetInst()->FindWidget<UPlayerStatusHUDWidget>(TEXT("PlayerStatusHUDWidget"));
			if (HUDWidget)
			{
				//아직 저장기능이 없으므로 현재 Exp 는 케릭터 초기값으로 계산한다
				int32 ExpToNextLevel = ExpInfo->RequiredExp;
				HUDWidget->SetXPBar(mExp, ExpToNextLevel);
			}
		}
	}

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
	mAnimInst = Cast<UPlayerTemplateAnimInstance>(GetMesh()->GetAnimInstance());


	//오버렙,힛 이벤트 델리게이팅
		//웨폰 콜리전 박스가 있는 경우 모두에 델리게이트한다
	if (mWeaponColBox.Num() > 0)
	{
		int32 Count = mWeaponColBox.Num();
		for (int32 i = 0; i < Count; i++)
		{
			//Overlap
			UE_LOG(TestGame, Warning, TEXT("Overlap delegate bound : mWweaponColBox[%d]"), i);
			mWeaponColBox[i]->OnComponentBeginOverlap.AddDynamic(
				this, &APlayerCharacter::OnWeaponOverlap);

			//Onhit
			//UE_LOG(TestGame, Warning, TEXT("OnHit delegate bound : mWweaponColBox[%d]"), i);
			//mWeaponColBox[i]->OnComponentHit.AddDynamic(
			//	this, &APlayerCharacter::OnWeaponHit);
		}
	}
}

void APlayerCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	//const UPlayerData* PlayerData =
	//	GetDefault<UPlayerData>();

	//if (PlayerData)
	//{
	//	FPlayerTableInfo* TableInfo =
	//		PlayerData->FindPlayerInfo(mDataKey);

	//	if (TableInfo)
	//	{
	//		mJob = TableInfo->Job;
	//		mName = TableInfo->Name;
	//		mAttack = TableInfo->Attack;
	//		mDefense = TableInfo->Defense;
	//		mHP = TableInfo->HP;
	//		mHPMax = TableInfo->HP;
	//		mMP = TableInfo->MP;
	//		mMPMax = TableInfo->MP;
	//		mAttackDistance = TableInfo->AttackDistance;
	//		mMoveSpeed = TableInfo->MoveSpeed;
	//		mLevel = TableInfo->Level;
	//		mExp = TableInfo->Exp;
	//		mGold = TableInfo->Gold;

	//		GetCharacterMovement()->MaxWalkSpeed = mMoveSpeed;

	//		//mMovement->SetMaxSpeed(mMoveSpeed);

	//		UE_LOG(TestGame, Warning, TEXT("Player Info"));
	//	}
	//}
}

/*BindAction 으로 바인딩 되어 있으며,
입력액션 'IAMove' 가 Triggered 될 때 호출되는 함수이다.*/
void APlayerCharacter::OnIAMoveTriggered(const FInputActionValue& Value)
{
	//이동 방식 2. (메시 중심 : 현재 메시가 보는 방향이 앞(W)  wasd 또는 왼쪽스틱의 방향을 판별한다)
	//이동 가능할 때만 실행
		if (!IsCharacterImmobilized())
		{
			FVector Axis;
			switch (Value.GetValueType())
			{
			case EInputActionValueType::Axis2D:
			{
				FVector2D Axis2D = Value.Get<FVector2D>();
				Axis.X = FMath::Clamp(Axis2D.X, -1.0f, 1.0f);
				Axis.Y = FMath::Clamp(Axis2D.Y, -1.0f, 1.0f);
				break;
			}
			default:
				return;
			}

			FVector MeshForward = GetMesh()->GetForwardVector();
			MeshForward.Z = 0;
			MeshForward.Normalize();

			FRotator AdjustedRotation = MeshForward.Rotation();
			AdjustedRotation.Yaw -= 90.0f;

			FVector AdjustedForward = AdjustedRotation.Vector();

			FVector Direction = AdjustedForward * -Axis.X;
			Direction += FVector::CrossProduct(AdjustedForward, FVector::UpVector) * Axis.Y;

			if (!Direction.IsNearlyZero())
			{
				Direction.Normalize();
			}
			//들고 있는 애님인스턴스가 있다면 
			//Axis 를
			//		: 입력으로 받은 X 및 Y 축 을  FVector 로 변환 한 값
			//애님인스턴스의 mDirection 멤버에 할당한다
			if (mAnimInst)
				mAnimInst->SetDirection(Axis);

			MoveToAxis(Direction, 
				FMath::Max(FMath::Abs(Axis.X), 
					FMath::Abs(Axis.Y)));
		}
		else
		{
			if (bDebugEnabled)
				UE_LOG(PlayerLog, Display, TEXT("Cannot Move Character : Immobillized"));
		}

		//이동방식 1. 카메라컴포넌트가 보는 방향 기준 (보는방향이 앞[W])
		////이동 가능할 때만 실행
		//if (!IsCharacterImmobilized())
		//{
		////	/*
		//	카메라가 보는 방향을 중심으로 동적으로 입력 방향이 바뀌게끔 이동하게 한다 , 이전 방식 (월드 기준 방식)은 아래 주석처리했다
		//	*/
		//	FVector Axis;
		//	switch (Value.GetValueType())
		//	{
		//	case EInputActionValueType::Axis2D:
		//	{
		//		FVector2D Axis2D = Value.Get<FVector2D>();
		//		Axis.X = FMath::Clamp(Axis2D.X, -1.0f, 1.0f);
		//		Axis.Y = FMath::Clamp(Axis2D.Y, -1.0f, 1.0f);
		//		break;
		//	}
		//	default:
		//		return;
		//	}

		//	FVector CameraDirection = mCamera->GetForwardVector();
		//	CameraDirection.Z = 0;
		//	CameraDirection.Normalize();

		//	FVector Direction = CameraDirection * Axis.X;
		//	Direction += FVector::CrossProduct(CameraDirection, FVector::UpVector) * Axis.Y;

		//	if (!Direction.IsNearlyZero())
		//	{
		//		Direction.Normalize();
		//	}

		//	MoveToAxis(Direction, FMath::Max(FMath::Abs(Axis.X), FMath::Abs(Axis.Y)));

		//	//들고 있는 애님인스턴스가 있다면 
		//	//Axis 를
		//	//		: 입력으로 받은 X 및 Y 축 을  FVector 로 변환 한 값
		//	//애님인스턴스의 mDirection 멤버에 할당한다
		//	if (mAnimInst)
		//		mAnimInst->SetDirection(Axis);
		//}
		//else
		//{
		//	if (bDebugEnabled)
		//		UE_LOG(PlayerLog, Display, TEXT("Cannot Move Character : Immobillized"));
		//}



	////구 이동 방식 (월드 중심, 케릭터의 초기 방향을 기준으로함 (월드 정반대방향이 앞)
	////이동 가능할 때만 실행
	//if (!IsCharacterImmobilized())
	//{
	//	/*
	//	인자로 받아오는 값은 입력 액션 설정과 매핑 컨텍스트에서 정의한 내용을 따른다
	//		정의
	//			IA_Move 는 2D 형식으로 정의하고 있다
	//			매핑컨텍스트에서는
	//				키보드 입력을 X 축과 Y 축(스위즐 된 경우) 으로 매핑하였다
	//			Triggered 라면 1 (부정 인 경우 -1)
	//						 아니면 0 이다*/

	//						 //입력값을 벡터로 변환한 값을 저장할 변수를 선언한다
	//						 //벡터 계산을 위해 쓰일 것이다.
	//	FVector Axis;

	//	//입력에서 전달된 값을 벡터로 형변한다.
	//	//Value 값의 예외 처리를 위해 switch 문의 형태로 구현했다
	//	switch (Value.GetValueType())
	//	{
	//	case EInputActionValueType::Axis1D:
	//		Axis.X = FMath::Clamp(Value.Get<float>(), -1.0f, 1.0f);
	//		if (bDebugEnabled)
	//			UE_LOG(PlayerLog, Display,
	//				TEXT("InputActionType [Axis1D],\t %.1f"),
	//				Axis.X);
	//		break;
	//	case EInputActionValueType::Axis2D:
	//	{
	//		FVector2D Axis2D = Value.Get<FVector2D>();
	//		Axis.X = FMath::Clamp(Axis2D.X, -1.0f, 1.0f);
	//		Axis.Y = FMath::Clamp(Axis2D.Y, -1.0f, 1.0f);
	//		if (bDebugEnabled)
	//			UE_LOG(PlayerLog, Display,
	//				TEXT("InputActionType [Axis2D],\t( %.1f, %.1f )"),
	//				Axis.X, Axis.Y);
	//		break;
	//	}
	//	case EInputActionValueType::Axis3D:
	//		Axis = Value.Get<FVector>();
	//		if (bDebugEnabled)
	//			UE_LOG(PlayerLog, Display,
	//				TEXT(" InputActionType [Axis3D],\t( %.1f, %.1f, %.1f )"),
	//				Axis.X, Axis.Y, Axis.Z);
	//		break;
	//	default:
	//		if (bDebugEnabled)
	//			UE_LOG(LogTemp, Warning,
	//				TEXT("Unexpected InputActionValueType received: %d"),
	//				static_cast<int32>(Value.GetValueType()));
	//		return;
	//	}

	//	// 방향을 계산하기위한 FVector 객체이다. 0으로 초기화한다
	//	FVector Direction = FVector::Zero();
	//	
	//	//X 축의 값을 받아 현재엑터의 전방벡터 (X축) 에 곱한다
	//	//Y 축의 값을 받아 현재 엑터의 우측벡터 (Y축) 에 곱하여 모든 값을 저장한다
	//	Direction = GetActorForwardVector() * Axis.X;
	//	Direction += GetActorRightVector() * Axis.Y;

	//	/* 또는
	//	 Direction = FVector(GetActorForwardVector() * Axis.X,
	//									GetActorRightVector() * Axis.Y,
	//									GetActorUpVector());							*/

	//									//정규화를 하여 대각선의 입력을 매끄럽게 처리한다.
	//									//예외 처리 0 정규화를 방지한다
	//	if (!Direction.IsNearlyZero())
	//	{
	//		//예외 처리 부동소수점 연산오류(아주 작은 값 등) 를 방지한다
	//		const float Threshold = KINDA_SMALL_NUMBER;
	//		if (Direction.SizeSquared() > Threshold)
	//		{
	//			Direction.Normalize();
	//			//예외 처리 정규화 후 크기 보정
	//			Direction *= 1.0f / FMath::Max(Direction.Size(), 1.0f);
	//			if (bDebugEnabled)
	//				UE_LOG(PlayerLog, Display, TEXT("Direction Normalized Safety : ( %.1f, %.1f, %.1f )"), Direction.X, Direction.Y, Direction.Z);
	//		}
	//	}


	//	//mAnimInst 를 BeginPlay 에서 받아오므로 객체를 생성할 필요가 없다.
	//		////현재 플레이어 메쉬의 애님인스턴스를 받아 
	//		//// UAnimInstancePlayer 로 캐스팅한다
	//		//UAnimInstancePlayer* AnimInst =
	//		//	Cast<UAnimInstancePlayer>(GetMesh()->GetAnimInstance());
	//
	//	//들고 있는 애님인스턴스가 있다면 
	//	//Axis 를
	//	//		: 입력으로 받은 X 및 Y 축 을  FVector 로 변환 한 값
	//	//애님인스턴스의 mDirection 멤버에 할당한다
	//	if (mAnimInst)
	//		mAnimInst->SetDirection(Axis);

	//	//계산 완료된 벡터로 아래의 함수를 호출한다
	//	//MoveToAxis 는 이동을 처리하며 이동과 관련된 추가 기능을 구현할 수 있도록 
	//	//사용자 정의로 작성한 멤버 함수이다
	//	float ScaleValue = FMath::Max(
	//		FMath::Abs(Axis.X),
	//		FMath::Abs(Axis.Y)
	//	);

	//	MoveToAxis(Direction, ScaleValue);
	//}
	//else
	//{
	//	if (bDebugEnabled)
	//		UE_LOG(PlayerLog, Display, TEXT("Cannot Move Character : Immobillized"));
	//}
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

//이제 일반 공격도 1234567 중에 하게 해야 할듯 왼쪽 클릭은 엑터 선택으로 분리하자
void APlayerCharacter::OnIAAttackStarted(const FInputActionValue& Value)
{
	//행위가 가능할 때만 공격을 한다 예외. 스턴/다른액팅/높은데서의착지모션 등 
	if (CanCharacterAct())
	{
			//애님인스턴스의 PlayAttack 을 호출한다
			mAnimInst->PlayAttack();
	}
}

void APlayerCharacter::OnIAAttackTriggered(const FInputActionValue& Value)
{
	//목표지역 선택 중이 아닐때만 실행
	/*if (!GetPreviewCastActivation())
	OnIAAttackStarted(Value);*/
	//행위가 가능할 때만 공격을 한다 예외. 스턴/다른액팅/높은데서의착지모션 등 
	if (CanCharacterAct())
	{
		//애님인스턴스의 PlayAttack 을 호출한다
		mAnimInst->PlayAttack();
	}
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
		if(!bIsRightMouseButtonPressed)
			mAnimInst->SetCamRotateByInput(FinalQuat.Rotator());
	
		// 추가: 마우스 우클릭 상태라면 캐릭터 메쉬도 회전
		if (bIsRightMouseButtonPressed && !(mAnimInst->HasPlayerAnim(EPlayerAnim::Dash)))
		{
			FRotator FinalRotation = FinalQuat.Rotator();
			FRotator MeshRotation = GetMesh()->GetRelativeRotation();
			//Yaw 값만 반영
			MeshRotation.Yaw = FinalRotation.Yaw - 90.f;  //메쉬 방향 월드 불일치 보정
			GetMesh()->SetRelativeRotation(MeshRotation);
		}

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

void APlayerCharacter::OnIACharacterRotateTriggered(const FInputActionValue& Value)
{
	bIsRightMouseButtonPressed = true;
}

void APlayerCharacter::OnIACharacterRotateCompleted(const FInputActionValue& Value)
{
	bIsRightMouseButtonPressed = false;
}

void APlayerCharacter::OnIADashStarted(const FInputActionValue& Value)
{
	if (IsCharacterImmobilized() ||
		mCharacterState.HasState(mCharacterState.ACTING)
		|| mAnimInst->GetPlayerAnim() == EPlayerAnim::Dash)
		//해당 속도의 카메라 방향으로 대시를 한다
		return;
	else
		//Dash Sfx 를 재생시킨다
		UGameplayStatics::PlaySoundAtLocation(GetWorld(),
			mSfxDash,
			GetActorLocation());
	//실제로 이동을 시킨다
	
	//잔상을 만들어내기 위하여 주기적으로 타이머를 생성한다
	if (!mDashAfterImageTimerHandler.IsValid())
	{
		GetWorldTimerManager().SetTimer(mDashAfterImageTimerHandler, this, &APlayerCharacter::AfterImageLoop, 0.03f, true, 0.01f);
		// 타이머해제
		// GetWorldTimerManager().ClearTimer(mDashAfterImageTimerHandler);
	}

		Dash(2000.f);
}

void APlayerCharacter::OnIASprintTriggered(const FInputActionValue& Value)
{
	Sprint();
}

void APlayerCharacter::OnIASprintCompleted(const FInputActionValue& Value)
{
	StopSprint();
}

void APlayerCharacter::OnIASpecialStarted(const FInputActionValue& Value)
{
	if (!CanCharacterAct() ||
		mCharacterState.HasState(mCharacterState.ACTING)
		|| mAnimInst->GetPlayerAnim() == EPlayerAnim::Special)
	{
		return;
	}
		mCharacterState.SetState(mCharacterState.ACTING);
		mAnimInst->SetPlayerAnim(EPlayerAnim::Special);
		SpecialAttack();
		mAnimInst->PlaySpecial();
}

void APlayerCharacter::OnIAPreviewThanCastStarted(const FInputActionValue& Value)
{
	//Value.GetValueType
	
	//행위가 가능한 경우에만 사용한다
	if (!CanCharacterAct())
	{
		return;
	}
		//범위를 선택 중인 경우 범위표시를 해제한다 (자식클래스에서 재정의)
		if (GetPreviewCastActivation())
		{	
			UE_LOG(TestGame, Display, TEXT("An selecting zone has been canceled by player"));
			CancelPreviewCastZone();
			SetPreviewCastActivation(false);
			return;
		}
		//범위 선택 중이 아닌 경우 범위를 표시한다 (자식클래스에서 재정의)
		else
		{
			UE_LOG(TestGame, Display, TEXT("Select zone cast a skill"));
			SetPreviewCastActivation(true);
			MakePreviewCastZone();
		}
}

void APlayerCharacter::OnIAAcceptCastStarted(const FInputActionValue& Value)
{
	//액션 불능인경우 리턴
	if (!CanCharacterAct())
		return;

	//목표지역 선택중 -> 스킬 컨펌
	if(GetPreviewCastActivation())
	{
		UE_LOG(TestGame, Display, TEXT("Accept Preview Cast Zone"));
		AcceptPreviewCastZone();
		SetPreviewCastActivation(false);
	}
}


void APlayerCharacter::OnIACancelCastStarted(const FInputActionValue& Value)
{
	//액션 불능인경우 리턴
	if (!CanCharacterAct())
		return;

	//목표지역 선택중 -> 취소 
	if (GetPreviewCastActivation())
	{
		UE_LOG(TestGame, Display, TEXT("Cancel Preview Cast Zone"));
		CancelPreviewCastZone();
		SetPreviewCastActivation(false);
	}
}

void APlayerCharacter::OnIACleaveStarted(const FInputActionValue& Value)
{
	if (!CanCharacterAct() || mAnimInst->GetPlayerAnim() == EPlayerAnim::Cleave)
	{
		return;
	}
	mCharacterState.SetState(mCharacterState.ACTING);
	mAnimInst->SetPlayerAnim(EPlayerAnim::Cleave);
	CleaveAttack();
	mAnimInst->PlayCleave();
}

void APlayerCharacter::OnIAPlayerInfoPanelStarted(const FInputActionValue& Value)
{
	UE_LOG(TestGame, Warning, TEXT("OnIAPlayerInfoPanelStarted --> "));
	
	//1. 싱글턴 매니저로부터 Panel 위젯을 받아온다
	UMainWidget* MainWidget = CWidgetManager::GetInst()->FindWidget<UMainWidget>(TEXT("MainWidget"));
	
	if (MainWidget)
	{
		//켜지는 경우는 정보를 업데이트 한다
		if (MainWidget->GetCharacterInfoPanelVisibility() == ESlateVisibility::Collapsed)
		{
			UCharacterInfoWidget* InfoPanelWidget = CWidgetManager::GetInst()->FindWidget< UCharacterInfoWidget>(TEXT("CharacterInfoWidget"));
			if (InfoPanelWidget)
			{
				ICharacterPublicInterface* Info = Cast<ICharacterPublicInterface>(this);
				if(Info)
					InfoPanelWidget->UpdatePlayerInfo(Info);
			}
		}

		// main -> ToggleCharacterInfoPanel()
		MainWidget->ToggleCharacterInfoPanel();
		// main -> GetCharacterInfoPanelVisibility()
	}
	UE_LOG(TestGame, Warning, TEXT("\t\t -->  CharacterInfoWidget Not Found"));
}

void APlayerCharacter::OnIASelectCharacterStarted(const FInputActionValue& Value)
{
	//범위 선택중 아니면 선택도 병행으로 실행
	if (!GetPreviewCastActivation())
	{
		//마우스 엑터 충돌이 있을 수 있으니 체크한다
		APlayerControllerTestGame* PC = Cast<APlayerControllerTestGame>(GetWorld()->GetFirstPlayerController());
		if (PC)
		{
			PC->SelectNPC();
		}
	}
}

void APlayerCharacter::OnIAPanelSkillStarted(const FInputActionValue& Value)
{
	CWidgetManager::GetInst()->FindWidget<UMainWidget>(TEXT("MainWidget"))->ButtonSkillOnClicked();
}

void APlayerCharacter::OnIAPanelInvStarted(const FInputActionValue& Value)
{
	CWidgetManager::GetInst()->FindWidget<UMainWidget>(TEXT("MainWidget"))->ButtonInvOnClicked();
}

void APlayerCharacter::MoveToAxis(const FVector& Axis, const float ScaleValue)
{
	if (FMath::Abs(ScaleValue) > KINDA_SMALL_NUMBER)
		AddMovementInput(Axis, ScaleValue);
	else
		AddMovementInput(Axis);
}
void APlayerCharacter::NormalAttack()
{

}

void APlayerCharacter::SpecialAttack()
{

}

void APlayerCharacter::CleaveAttack()
{
}

void APlayerCharacter::Dash(float DashSpeed)
{
	//케릭터무브먼트컴포넌트가 있고  
	// Immobilized 상태가 아니며, 대시중이 아닐때만 시전한다
	if (GetCharacterMovement() && !IsCharacterImmobilized() && !(mAnimInst->HasPlayerAnim(EPlayerAnim::Dash)))
	{
		mCharacterState.SetState(mCharacterState.ACTING);
		//애님인스턴스의 PlayDash 를 호출한다
		mAnimInst->PlayDash();

		////지상에 있을 경우 충돌(블럭) 등으로 대시거리가 짧기 떄문에
		////일정 승수로 보정을 해준다  
		//if (GetCharacterMovement()->IsMovingOnGround())
		//	DashSpeed *= 5.0f;

		FVector DashDirection;
	
		if (!GetCharacterMovement()->Velocity.IsNearlyZero() && 
			GetCharacterMovement()->IsMovingOnGround())
		{
			// 바닥에서 이동 중이라면 해당 방향을 기준으로 잡는다
			DashDirection = GetCharacterMovement()->Velocity.GetSafeNormal();
			DashDirection.Z = 0;
			DashDirection.Normalize();
		}

		else
		{
			//그라운드 이동중이 아니라면 카메라 방향을 기준으로 잡는다
			DashDirection = mCamera->GetForwardVector();
			DashDirection.Z = 0;
			DashDirection.Normalize();

			//메쉬의 좌우 회전값을 대쉬 방향으로 하기 위한 처리이다
			FRotator MeshRotation = DashDirection.Rotation();
			//메쉬가 기본적으로 -90 Yaw 만큼 보정된 상태이므로 회전값에 반영해준다
			MeshRotation.Yaw -= 90.0f; // Yaw 보정
			MeshRotation.Pitch = 0.f;
			MeshRotation.Roll = 0.f;

			//계산된 회전값으로 메쉬를 회전시킨다
			GetMesh()->SetWorldRotation(MeshRotation);
		}

		//지형 기울기 기반 보정 -> 내적구해서 
		FHitResult Hit;
		FVector Start = GetActorLocation();
		FVector End = Start - FVector(0, 0, 1200.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
		{
			FVector SurfaceNormal = Hit.ImpactNormal;
			float SlopeFactor = FVector::DotProduct(SurfaceNormal, FVector::UpVector); // 1.0 = 평지, 0.0 = 수직면

			float DashBoost = FMath::Clamp(SlopeFactor, 0.3f, 1.0f); // 경사에 따라 최소~최대 보정
			DashSpeed *= DashBoost;
		}

		//전달받은 인자가 대시의 속도와 길이를 결정한다
		FVector DashVelocity = DashDirection * DashSpeed;
		LaunchCharacter(DashVelocity, true, true);
	}
}

void RestoreGravity()
{
	
}

void APlayerCharacter::Sprint()
{
	if (GetCharacterMovement())
	{
		//GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed * 1.4f; // 이동속도를 곱한만큼 빨라지게 함
	}
}

void APlayerCharacter::StopSprint()
{
	if (GetCharacterMovement())
	{
		//GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed; // 원래 속도로 복귀
	}
}

void APlayerCharacter::MakePreviewCastZone()
{
}

void APlayerCharacter::CancelPreviewCastZone()
{
}

void APlayerCharacter::AcceptPreviewCastZone()
{
}

void APlayerCharacter::InitCastZoneSkill()
{
}

void APlayerCharacter::AfterImageLoop()
{
	double  SpeedThresold = (double)(GetCharacterMovement()->MaxWalkSpeed * 1.2f);
	//현재 케릭터의 속도를 체크해서 잔상이 이동중에만 남게 추가 처리한다
	if (GetVelocity().Size() > SpeedThresold)
	{
		FActorSpawnParameters	param;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FVector Location = GetActorLocation();

		Location.Z -= GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		FRotator	RotMesh = GetMesh()->GetComponentRotation();
		AAfterImage* Effect = GetWorld()->SpawnActor<AAfterImage>(Location, RotMesh, param);

		Effect->CopyImage(GetMesh());

		UE_LOG(TestGame, Warning, TEXT("AfterImage Spawn Called"));
	}
	else
		UE_LOG(TestGame, Warning, TEXT("speed does not match to Thresold[%0.3f : %0.3f]"), SpeedThresold, GetVelocity().Size());
}

void APlayerCharacter::RunSweepConeAndDamage(float Damage, float SphereSize, float SphereDistance, float AngleStartDistance, float InnerAngle, ECollisionChannel TraceChannel, UNiagaraSystem* VFXAsset, bool bSpawnVFXOnHit, USoundBase* SFXAsset, bool bSpawnSFXOnHit)
{
	//SphereSize				생성할 구의 크기
	//SphereDistance			구를 생성할 거리
	//AngleStartDistance		각도를 재기 시작할 거리
	//InnerAngle				조건으로 검출할 각도 (단측면의 값임 총 각도는 * 2 )
	//TraceChannel				충돌 계산할 채널
	//HitVFX					충돌 시 재생할 VFX 에셋
	//HitSFX					충돌 시 재생할 SFX 에셋
	//bSpawnVFXEveryHit	모든 충돌에 VFX 스폰
	//bSpawnSFXEveryHit	모든 충돌지점에 SFX 스폰

	if (VFXAsset && !bSpawnVFXOnHit)
	{
		//VFX 에셋을 전달받았고  충돌체에 발생않음인 경우  캐릭터에 VFX 를 1회 스폰한다
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFXAsset, GetMesh()->GetComponentLocation());
	}
	if (SFXAsset && !bSpawnSFXOnHit)
	{
		//SFX 에셋을 전달받았고  충돌체에 발생않음인 경우  캐릭터에 SFX 를 1회 스폰한다
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SFXAsset,
			GetMesh()->GetComponentLocation(), FRotator::ZeroRotator,
			2.f);
	}

	// 1. 케릭터의 전방 방향을 구하고 각도를 계산할 위치를 잡아준다Reference Point
	FVector MeshForwardVector = GetMesh()->GetForwardVector();
	//* 메쉬의 Yaw 회전 틀어짐 보정 90f
	FVector AdjustedForward = MeshForwardVector.RotateAngleAxis(90.0f, FVector(0, 0, 1));
	//
	FVector ReferencePoint = GetActorLocation() + (AdjustedForward * AngleStartDistance);

	//구를 생성할 중심점을 만든다   =   SpherePoint
	FVector SpherePoint = GetActorLocation() + (AdjustedForward * (SphereDistance)); // Sweep 시작 위치
	FVector SweepEnd = SpherePoint;

	// 2. 설정한 반경(SphereSize) 크기의 스피어 Sweep 을 실행한다
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereSize);
	TArray<FHitResult> HitResults;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		SpherePoint,														//구를 만들 시작 포인트
		SweepEnd,														//종료 포인트 (구체이므로 같은곳을 지정하였다
		FQuat::Identity,
		TraceChannel,		// PlayerWeaponSweep -> 트레이스 채널
		SphereShape														//SphereSize 크기의 Sphere 로 만든다
	);
	if (CVarDebugLines.GetValueOnGameThread() == 1) // 값이 1이면 디버그 출력
	{

	// 스피어 디버그 드로잉 (Sweep 검사 범위)
	float DrawTime = 2.0f;
	DrawDebugSphere(
		GetWorld(),
		SpherePoint,					// 구체 중심
		SphereSize,					// 반지름
		12,								// 세그먼트 수 (디테일 정도)
		FColor::Red,					// 색상
		false,
		DrawTime 						// 지속 시간
	);

	// 전방 기준점 디버그 드로잉
	DrawDebugPoint(
		GetWorld(),
		ReferencePoint,
		10.0f,							// 크기
		FColor::Blue,
		false,
		DrawTime
	);

	// Cone 판정 영역 디버그 드로잉 (각도 표시)
	FVector LeftLimit = AdjustedForward.RotateAngleAxis(-InnerAngle, FVector(0, 0, 1));			// -각도 방향(좌측각)
	FVector RightLimit = AdjustedForward.RotateAngleAxis(InnerAngle, FVector(0, 0, 1));			// +각도 방향(우측각)
		DrawDebugLine(GetWorld(), ReferencePoint, ReferencePoint + LeftLimit * SphereSize, FColor::Green, false, DrawTime);
		DrawDebugLine(GetWorld(), ReferencePoint, ReferencePoint + RightLimit * SphereSize, FColor::Green, false, DrawTime);
	}
	//최적화 : 충돌 없으면 나가기
	if (!bHit)
		return;

	// 3. 모든 충돌체 탐색
	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		//최적화 : 충돌 가능한 엑터가 아니면 건너뛰기 
		if (!HitActor)
			continue;

		// 4. 기준 지점(ReferencePoint)에서 충돌체 방향을 계산 (정규화로 정확도 보장)
		FVector ToHitActor = (HitActor->GetActorLocation() - ReferencePoint).GetSafeNormal();
		// 내적구하기 ForwardVector vs ToHitActor
		float DotProduct = FVector::DotProduct(AdjustedForward, ToHitActor);

		//비교 방식 :  두 값(내적 vs 각도) 을 비교가능하도록 같은 성질로 만들어야 한다
		// 방식 1. 라디안 DotProduct 을 각도로 변환하기 (역코사인)
		float Angle = FMath::Acos(DotProduct) * (180.0f / PI);
		// 방식 2. 각도 InnerAngle 을 라디안으로 변환하기 (코사인)
		//			float TargetRadian = FMath::Cos(FMath::DegreesToRadians(InnerAngle));
		

		//판정 조건에 의해 검출된 엑터에 대한 구현 
		//목표가 cone 모양이므로 음수를 걸러내야햔다 (음수는 케릭터 뒷방향이므로..)
		if (Angle <= InnerAngle && Angle >= 0)	
		{
			//몬스터 및 하위 클래스 인 경우에만 처리 처리
			if (AMonster* Monster = Cast<AMonster>(HitActor))
			{
				if (VFXAsset && bSpawnVFXOnHit)
				{
					FVector	VFXLocation = Monster->GetMeshComponent()->GetSocketLocation(TEXT("Chest"));
					if(VFXLocation == FVector::ZeroVector)
					{
						//VFX 에셋을 전달받고 모든 타깃에 VFX 스폰을 전달받은 경우
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFXAsset, Hit.ImpactPoint);
					}
					else
					{
						UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFXAsset, VFXLocation);
					}
				}
				if (SFXAsset && bSpawnSFXOnHit)
				{
					//SFX 에셋을 전달받고 모든 타깃에 SFX 스폰을 전달받은 경우
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SFXAsset,
						Hit.ImpactPoint, FRotator::ZeroRotator,
						2.f);
				}

				UE_LOG(TestGame, Warning, TEXT("Actor [%s] has in inside of %0.2f degrees"), *HitActor->GetName(), InnerAngle);

				//충돌 받은 엑터에 데미지 주기
				FDamageEvent	DmgEvent;
				HitActor->TakeDamage(Damage, DmgEvent, GetController(), this);
			}
		}
	}
}

void APlayerCharacter::RunSweepConeAndDamage(float Damage, float SphereSize, float SphereDistance, float AngleStartDistance, float InnerAngle, ECollisionChannel TraceChannel, UParticleSystem* VFXAsset, bool bSpawnVFXOnHit, USoundBase* SFXAsset, bool bSpawnSFXOnHit)
{
	// VFX 및 SFX 재생  = 파티클시스템
	if (VFXAsset && !bSpawnVFXOnHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			VFXAsset, GetMesh()->GetComponentLocation(),
			FRotator::ZeroRotator);
	}
	if (SFXAsset && !bSpawnSFXOnHit)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SFXAsset,
			GetMesh()->GetComponentLocation(), FRotator::ZeroRotator,
			2.f);
	}

	// 1. 케릭터의 전방 방향을 구하고 각도를 계산할 위치를 잡아준다Reference Point
	FVector MeshForwardVector = GetMesh()->GetForwardVector();
	//* 메쉬의 Yaw 회전 틀어짐 보정 90f
	FVector AdjustedForward = MeshForwardVector.RotateAngleAxis(90.0f, FVector(0, 0, 1));
	FVector ReferencePoint = GetActorLocation() + (AdjustedForward * AngleStartDistance);

	//구를 생성할 중심점을 만든다   =   SpherePoint
	FVector SpherePoint = GetActorLocation() + (AdjustedForward * (SphereDistance)); // Sweep 시작 위치
	FVector SweepEnd = SpherePoint;

	// 2. 설정한 반경(SphereSize) 크기의 스피어 Sweep 을 실행한다
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereSize);
	TArray<FHitResult> HitResults;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		SpherePoint,														//구를 만들 시작 포인트
		SweepEnd,														//종료 포인트 (구체이므로 같은곳을 지정하였다
		FQuat::Identity,
		TraceChannel,													//트레이스 채널
		SphereShape														//SphereSize 크기의 Sphere 로 만든다
	);
	if (CVarDebugLines.GetValueOnGameThread() == 1) // 값이 1이면 디버그 출력
	{

	// 스피어 디버그 드로잉 (Sweep 검사 범위)
	float DrawTime = 2.0f;
	DrawDebugSphere(
		GetWorld(),
		SpherePoint,					// 구체 중심
		SphereSize,					// 반지름
		12,								// 세그먼트 수 (디테일 정도)
		FColor::Red,					// 색상
		false,
		DrawTime 						// 지속 시간
	);

	// 전방 기준점 디버그 드로잉
	DrawDebugPoint(
		GetWorld(),
		ReferencePoint,
		10.0f,							// 크기
		FColor::Blue,
		false,
		DrawTime
	);

	// Cone 판정 영역 디버그 드로잉 (각도 표시)
	FVector LeftLimit = AdjustedForward.RotateAngleAxis(-InnerAngle, FVector(0, 0, 1));			// -각도 방향(좌측각)
	FVector RightLimit = AdjustedForward.RotateAngleAxis(InnerAngle, FVector(0, 0, 1));			// +각도 방향(우측각)
		DrawDebugLine(GetWorld(), ReferencePoint, ReferencePoint + LeftLimit * SphereSize, FColor::Green, false, DrawTime);
		DrawDebugLine(GetWorld(), ReferencePoint, ReferencePoint + RightLimit * SphereSize, FColor::Green, false, DrawTime);
	}
	//최적화 : 충돌 없으면 나가기
	if (!bHit)
		return;

	// 3. 모든 충돌체 탐색
	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		//최적화 : 충돌 가능한 엑터가 아니면 건너뛰기 
		if (!HitActor)
			continue;

		// 4. 기준 지점(ReferencePoint)에서 충돌체 방향을 계산 (정규화로 정확도 보장)
		FVector ToHitActor = (HitActor->GetActorLocation() - ReferencePoint).GetSafeNormal();
		// 내적구하기 ForwardVector vs ToHitActor
		float DotProduct = FVector::DotProduct(AdjustedForward, ToHitActor);

		//비교 방식 :  두 값(내적 vs 각도) 을 비교가능하도록 같은 성질로 만들어야 한다
		// 방식 1. 라디안 DotProduct 을 각도로 변환하기 (역코사인)
		float Angle = FMath::Acos(DotProduct) * (180.0f / PI);
		// 방식 2. 각도 InnerAngle 을 라디안으로 변환하기 (코사인)
		//			float TargetRadian = FMath::Cos(FMath::DegreesToRadians(InnerAngle));

		//판정 조건에 의해 검출된 엑터에 대한 구현 
		//목표가 cone 모양이므로 음수를 걸러내야햔다 (음수는 케릭터 뒷방향이므로..)
		if (Angle <= InnerAngle && Angle >= 0)
		{
			//몬스터 및 하위 클래스 인 경우에만 처리 처리
			if (AMonster* Monster = Cast<AMonster>(HitActor))
			{
				if (VFXAsset && bSpawnVFXOnHit)
				{
					FVector	VFXLocation = Monster->GetMeshComponent()->GetSocketLocation(TEXT("Chest"));
					if (VFXLocation == FVector::ZeroVector)
					{
						//VFX 에셋을 전달받고 모든 타깃에 VFX 스폰을 전달받은 경우
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
							VFXAsset, Hit.ImpactPoint,
							Hit.ImpactNormal.Rotation());
					}
					else
					{
						//VFX 에셋을 전달받고 모든 타깃에 VFX 스폰을 전달받은 경우
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
							VFXAsset, VFXLocation,
							Hit.ImpactNormal.Rotation());
					}
					
				}
				if (SFXAsset && bSpawnSFXOnHit)
				{
					//SFX 에셋을 전달받고 모든 타깃에 SFX 스폰을 전달받은 경우
					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SFXAsset,
						Hit.ImpactPoint, FRotator::ZeroRotator,
						2.f);
				}

				UE_LOG(TestGame, Warning, TEXT("Actor [%s] has in inside of %0.2f degrees"), *HitActor->GetName(), InnerAngle);

				//충돌 받은 엑터에 데미지 주기
				FDamageEvent	DmgEvent;
				HitActor->TakeDamage(Damage, DmgEvent, GetController(), this);
			}
		}
	}
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
		
		//IA_Dash 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
		//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIADash,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::OnIADashStarted);

		//IA_Sprint 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Triggered 및 Completed 두 형식으로 호출한다.
		//		--> 플레이어캐릭터의 멤버함수 OnIAAttackTriggered 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIASprint,
				ETriggerEvent::Triggered,
				this,
				&APlayerCharacter::OnIASprintTriggered);
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIASprint,
				ETriggerEvent::Completed,
				this,
				&APlayerCharacter::OnIASprintCompleted);

		//IA_Special 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
		//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIASpecial,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::OnIASpecialStarted);

		//IA_CameraRotate 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Triggered 형식으로 호출한다.
		//		--> 플레이어캐릭터의 멤버함수 OnIACamRotateTriggered 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIACharacterRotate,
				ETriggerEvent::Triggered,
				this,
				&APlayerCharacter::OnIACharacterRotateTriggered);
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIACharacterRotate,
				ETriggerEvent::Completed,
				this,
				&APlayerCharacter::OnIACharacterRotateCompleted);

		//IA_Cleave 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
		//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIACleave,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::OnIACleaveStarted);

		//IA_PreviewThanCast 입력액션 에셋으로 함수를 바인딩한다 
		// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
		//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
			EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIAPreviewThanCast,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::OnIAPreviewThanCastStarted);

//IA_PreviewThanCast 입력액션 에셋으로 함수를 바인딩한다 
// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
			EnhancedInputComponent->
				BindAction(APCTestGameCDO->
					mIAAcceptCastZone,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::OnIAAcceptCastStarted);

			//IA_PreviewThanCast 입력액션 에셋으로 함수를 바인딩한다 
// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
			EnhancedInputComponent->
				BindAction(APCTestGameCDO->
					mIACancelCastZone,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::OnIACancelCastStarted);

//IA_PreviewThanCast 입력액션 에셋으로 함수를 바인딩한다 
// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
			EnhancedInputComponent->
				BindAction(APCTestGameCDO->
					mIAPlayerInfoPanel,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::OnIAPlayerInfoPanelStarted);

			//IA_SelectCharacter 입력액션 에셋으로 함수를 바인딩한다 
// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
			EnhancedInputComponent->
				BindAction(APCTestGameCDO->
					mIASelectCharacter,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::OnIASelectCharacterStarted);

			//IA_PanelSkill 입력액션 에셋으로 함수를 바인딩한다 
// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
			EnhancedInputComponent->
				BindAction(APCTestGameCDO->
					mIAPanelSkill,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::OnIAPanelSkillStarted);

			//IA_PanelSkill 입력액션 에셋으로 함수를 바인딩한다 
// 이벤트는 Started 형식으로 호출한다. 누르고 있어도 첫 입력 시에만 호출한다
//		--> 플레이어캐릭터의 멤버함수 OnIAAttackStarted 을 바인딩했다
			EnhancedInputComponent->
				BindAction(APCTestGameCDO->
					mIAPanelInv,
					ETriggerEvent::Started,
					this,
					&APlayerCharacter::OnIAPanelInvStarted);
	}
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
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

void APlayerCharacter::ApplyTrailEffect(bool bLeftHand, USkeletalMeshComponent* MeshComp, int32 CurrentTrackIndex)
{
	if (!MeshComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid MeshComp!"));
		return;
	}

	// 트레일을 적용할 본 소켓 결정
	FName SelectedSocketName = bLeftHand ? FName("weapon_l_trail_mid") : FName("weapon_r_trail_mid");
	if (bDebugEnabled)
	UE_LOG(LogTemp, Warning, TEXT("Selected Socket Name: %s"), *SelectedSocketName.ToString());

	// 트레일 가져오기 캐스케이드 (없으면 널)
	UParticleSystem* ParticleTrail
		= bLeftHand ? mTrailEffectL.Get() : mTrailEffectR.Get();
	// 트레일 가져오기 나이아가라
	UNiagaraSystem* NiagaraTrail = bLeftHand ? mTrailEffectNiagaraL.Get() : mTrailEffectNiagaraR.Get();

	// 트레일 적용 모드 확인
	if (GetTrailAssetMode() == ETrailAssetMode::NIAGARASYSTEM && NiagaraTrail)
	{
		//나이아가라 이펙트 스폰
		UNiagaraComponent* TrailComponent 
			= UNiagaraFunctionLibrary::SpawnSystemAttached(
				NiagaraTrail, 
				MeshComp, 
				SelectedSocketName,
			FVector::ZeroVector, 
				FRotator::ZeroRotator, 
				EAttachLocation::KeepRelativeOffset, true);
		//어떤 트랙에서 노티파이 이벤트가 나온건지 기억해두기 위해 태그를 추가한다
		TrailComponent->ComponentTags.Add(FName(*FString::FromInt(CurrentTrackIndex)));

		if (TrailComponent)
		{
			TrailComponent->SetFloatParameter(TEXT("TrailDuration"), 2.0f);  // 지속시간 예제
		}
		if (bDebugEnabled)
		UE_LOG(TestGame, Warning, TEXT("Niagara particle spawned"));
	}
	else if (GetTrailAssetMode() == ETrailAssetMode::PARTICLESYSTEM && ParticleTrail)
	{
		//이미터 스폰
		UParticleSystemComponent* TrailComponent 
			= UGameplayStatics::SpawnEmitterAttached(
				ParticleTrail, 
				MeshComp, 
				SelectedSocketName, 
				FVector::ZeroVector, 
				FRotator::ZeroRotator, 
				EAttachLocation::KeepRelativeOffset);
		
		//어떤 트랙에서 노티파이 이벤트가 나온건지 기억해두기 위해 태그를 추가한다
		TrailComponent->ComponentTags.Add(FName(*FString::FromInt(CurrentTrackIndex)));

		if (TrailComponent)
		{
			TrailComponent->SetFloatParameter(TEXT("TrailDuration"), 2.0f);
		}
		if (bDebugEnabled)
		UE_LOG(TestGame, Warning, TEXT("Particlesystem particle spawned"));
	}
}

void APlayerCharacter::OnWeaponOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult)
{
	//오버렙으로 처리하게끔 했을 경우에만 계산한다
	if (!bNormalAttackOverlap)
		return;

	UE_LOG(TestGame, Display, TEXT("OnWeaponOverlap entered"));
	////디버그 박스 그리기
	//if (bFromSweep) // 스윕 충돌이 발생했을 때만 실행
	//{
		//// 디버그 박스의 색상 및 두께 설정
		//FColor DebugColor = FColor::Red;
		//float DebugThickness = 2.0f;

		//// 충돌 지점 기준으로 박스 그리기
		//DrawDebugBox(
		//	GetWorld(),
		//	SweepResult.Location,			// 충돌 발생 위치에 그린다
		//	FVector(10.f, 10.f, 10.f),		// 박스 크기
		//	DebugColor,							// 색상
		//	false,									// 
		//	2.0f,										// 표시 시간 (초)
		//	0,
		//	DebugThickness // 선 두께
		//);
		//UE_LOG(LogTemp, Warning, TEXT("Collision at: %s"), *SweepResult.Location.ToString());
	/*UE_LOG(TestGame, Warning, TEXT("APlayerCharacter: Weapon Overlap Called, bFromSweep : %s, HitPoint : %s"), bFromSweep ? TEXT("true") : TEXT("false"), *SweepResult.ImpactPoint.ToString());*/
	//}

		//임시 충돌체를 만들어 (Sweep - By )   충돌 위치를 얻는 처리이다
		//매프레임 이 연산을 하지않고  오버렙 안에서만 추가 처리를 하는게 효율적이다
		//왜냐하면 무기의 형태에 유사하게 만들어낸 충돌체로부터 오버랩이 발생하는 것만으로도 유효한 충돌이 발생했음을 감지한 것이기 때문에 매프레임 sweep 을 검출할 이유가 없다 
		// 
		// 
		//메쉬에서 무기의 시작 또는 끝 위치에 소켓을 생성하고  
		//그 지점으로부터 충돌을 계산하기위한 선을 그려낸다 무기의 길이와 회전 방향에 일치하는 거리로 만들어야 한다
	FVector SweepStartPoint, SweepEndPoint, BoxExtent;

	//쌍수인지 검사
	if (mWeaponColBox.Num() > 0)
	{
		int32 Count = mWeaponColBox.Num();
		//쌍수인경우 현재 오버렙이 어떤손과 충돌했는지 구분하여 Start 와 End 의 대상소켓을 구한다
		if (Count >= 2)
		{
			for (int32 i = 0; i < Count; ++i)
			{
				if (mWeaponColBox[i] == OverlappedComponent)
				{
					SweepStartPoint = (i == 0) ? GetMesh()->GetSocketLocation(TEXT("weapon_r_sweep_start")) :
						GetMesh()->GetSocketLocation(TEXT("weapon_l_sweep_start"));
					SweepEndPoint = (i == 0) ? GetMesh()->GetSocketLocation(TEXT("weapon_r_sweep_end")) :
						GetMesh()->GetSocketLocation(TEXT("weapon_l_sweep_end"));
					//6 번인자 받기 
					BoxExtent = mWeaponColBox[i]->GetUnscaledBoxExtent();
				}
			}
		}
		else if (Count < 2)
		{
			//무기가 하나인 경우 
			SweepStartPoint = GetMesh()->GetSocketLocation(TEXT("weapon_sweep_start"));
			SweepEndPoint = GetMesh()->GetSocketLocation(TEXT("weapon_sweep_end"));
			BoxExtent = mWeaponColBox[0]->GetUnscaledBoxExtent();
		}
	}
	else
	{
		UE_LOG(TestGame, Warning, TEXT("No Collision box exists in this actor"));
		return;
	}

	// 시작점에서 끝점으로 향하는 방향을 만든다 -> GetSafeNormal 로 표준화
	FVector Direction = (SweepEndPoint - SweepStartPoint).GetSafeNormal();
	//해당방향으로 회전하는 방향을 만들어낸다
	FRotator RotDirection = Direction.Rotation();
	//쿼터니언회전으로 만든다 (sweep 에서 요구하는 것은 쿼터니언 회전이다)
	FQuat RotQuat = RotDirection.Quaternion();

	//7번 인자 param 생성
	FCollisionQueryParams	param(NAME_None, false, this);
	//param.AddIgnoreXXXX  로 제외목록 추가

	FHitResult		Hit;
	bool Collided = GetWorld()->SweepSingleByChannel(
		Hit,																	// 충돌 객체를 전달받을 객체
		SweepStartPoint,												// 계산 시작 지점
		SweepEndPoint,													// 계산 종료 지점
		RotQuat,															// 시작에서 종료지점을 향하는 쿼터니언 회전값 
		ECollisionChannel::ECC_GameTraceChannel7,		//충돌을 계산할 채널
		FCollisionShape::MakeBox(BoxExtent),				//충돌체의 모양
		param);																//추가 제외목록

	if (Collided)
	{
		if (CVarDebugLines.GetValueOnGameThread() == 1) // 값이 1이면 디버그 출력
		{

		// 디버그 박스의 색상 및 두께 설정
		FColor DebugColor = FColor::Red;
		float DebugThickness = 2.0f;

			// 충돌 지점 기준으로 박스 그리기
			DrawDebugBox(
				GetWorld(),
				Hit.ImpactPoint,			// 충돌 발생 위치에 그린다
				FVector(10.f, 10.f, 10.f),		// 박스 크기
				DebugColor,							// 색상
				false,									// 
				3.0f,										// 표시 시간 (초)
				0,
				DebugThickness // 선 두께
			);
		}
		UE_LOG(LogTemp, Warning, TEXT("Collision at: %s"), *Hit.ImpactPoint.ToString());

		//파티클 생성 : 동기화방식 런타임 로드  
		UParticleSystem* HitParticle = LoadObject<UParticleSystem>(
			GetWorld(),
			TEXT("/Script/Engine.ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/Primary/FX/p_CountessImpact.p_CountessImpact'"));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
			HitParticle, Hit.ImpactPoint,
			Hit.ImpactNormal.Rotation());

		//사운드생성 : 동기방식- 런타임 로드
		USoundBase* HitSound = LoadObject<USoundBase>(
			GetWorld(),
			TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_whipped01.sfx_whipped01'"));

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound,
			Hit.ImpactPoint, FRotator::ZeroRotator,
			2.f);
	}
}

void APlayerCharacter::OnWeaponHit(
	UPrimitiveComponent* HitComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	FVector NormalImpulse, 
	const FHitResult& HitResult)
{
	if (CVarDebugLines.GetValueOnGameThread() == 1) // 값이 1이면 디버그 출력
	{

		//디버그 박스 그리기
		// 디버그 박스의 색상 및 두께 설정
		FColor DebugColor = FColor::Red;
		float DebugThickness = 2.0f;

		// 충돌 지점 기준으로 박스 그리기
		DrawDebugBox(
			GetWorld(),
			HitResult.Location,			// 충돌 발생 위치에 그린다
			FVector(10.f, 10.f, 10.f),		// 박스 크기
			DebugColor,							// 색상
			false,									// 
			2.0f,										// 표시 시간 (초)
			0,
			DebugThickness // 선 두께
		);
	}
	UE_LOG(LogTemp, Warning, TEXT("Collision at: %s"), *HitResult.Location.ToString());

	UE_LOG(TestGame, Warning, TEXT("APlayerCharacter: Weapon Overlap Called, HitPoint : %s"),  *HitResult.ImpactPoint.ToString());
}

float APlayerCharacter::TakeDamage(float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent,
		EventInstigator, DamageCauser);

	if (DamageAmount > 0.f)
	{
		float	Dmg = DamageAmount - mDefense;
		Dmg = Dmg < 1.f ? 1.f : Dmg;

		Damage(Dmg);

		//HUD 에 받은 피해량을 반영해준다
			//1. 싱글턴 매니저로부터 해당 HUD 위젯의 주소를 가져온다 
		UPlayerStatusHUDWidget* HUDWidget = CWidgetManager::GetInst()->FindWidget<UPlayerStatusHUDWidget>(TEXT("PlayerStatusHUDWidget"));
		if (HUDWidget)
		{
			HUDWidget->SetHPBar(mHP, mHPMax);  //HP 바를 지금 클래스의 값으로 업데이트
		}
	}

	// 데미지 처리
	UE_LOG(TestGame, Warning, TEXT("Damage : %.5f"), DamageAmount);

	return DamageAmount;
}

void APlayerCharacter::SetGenericTeamId(
	const FGenericTeamId& TeamID)
{
	mTeamID = TeamID;
}

FGenericTeamId APlayerCharacter::GetGenericTeamId() const
{
	return mTeamID;
}

ETeamAttitude::Type APlayerCharacter::GetTeamAttitudeTowards(
	const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent =
		Cast<const IGenericTeamAgentInterface>(&Other);

	if (!OtherTeamAgent)
		return ETeamAttitude::Neutral;

	else if (OtherTeamAgent->GetGenericTeamId().GetId() == TeamNeutral)
		return ETeamAttitude::Neutral;

	return GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId() ?
		ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}

void APlayerCharacter::EnableOutLine(bool Enable)
{
	int32 Stencil = GetMesh()->CustomDepthStencilValue;

	// 스텐실 켜기로 호출 되었을 때
	if (Enable)
	{
		Stencil |= 1;
	}
	else
	{
		Stencil ^= 1;
	}
	//메쉬의 스텐실 뎁스에 값 적용
	GetMesh()->SetCustomDepthStencilValue(Stencil);
}

void APlayerCharacter::EnableOcclusion(bool Enable)
{
	int32 Stencil = GetMesh()->CustomDepthStencilValue;

	// 스텐실 켜기로 호출 되었을 때
	if (Enable)
	{
		Stencil |= 2;
	}
	else
	{
		Stencil ^= 2;
	}
	//메쉬의 스텐실 뎁스에 값 적용
	GetMesh()->SetCustomDepthStencilValue(Stencil);
}

void APlayerCharacter::UpdateHPBar()
{
	//HUD 에 사용한 마나를 반영한다
	//1. 싱글턴 매니저로부터 해당 HUD 위젯의 주소를 가져온다 
	UPlayerStatusHUDWidget* HUDWidget = CWidgetManager::GetInst()->FindWidget<UPlayerStatusHUDWidget>(TEXT("PlayerStatusHUDWidget"));
	if (HUDWidget)
	{
		HUDWidget->SetHPBar(mHP, mHPMax);  //HP 바를 업데이트
	}
}

void APlayerCharacter::UpdateMPBar()
{
	//HUD 에 사용한 마나를 반영한다
	//1. 싱글턴 매니저로부터 해당 HUD 위젯의 주소를 가져온다 
	UPlayerStatusHUDWidget* HUDWidget = CWidgetManager::GetInst()->FindWidget<UPlayerStatusHUDWidget>(TEXT("PlayerStatusHUDWidget"));
	if (HUDWidget)
	{
		HUDWidget->SetMPBar(mMP, mMPMax);  //MP 바를 업데이트
	}
}

void APlayerCharacter::OnGetExp(int32 AddNumber)
{

	AddExp(AddNumber);

	//현재 경험치 받아오기 
	int32 CurrentExp = GetExp();
	//exp 데이터 테이블 얻어오기
	const UPlayerData* PlayerData =
		GetDefault<UPlayerData>();

	if (PlayerData)
	{
		//다음레벨의 경험치 구성 정보 가져옴
		FLevelExpRow* ExpInfo = PlayerData->FindExpInfo(FName(*FString::FromInt(mLevel + 1)));

		if (ExpInfo)
		{
			//현재 누적 경험치가 다음레벨 경험치와 같거나 높으면 레벨업 처리
			if (CurrentExp >= ExpInfo->TotalExp)
				LevelUp();

			//HUD 에 경험치 값 반영시키기
				UPlayerStatusHUDWidget* HUDWidget = CWidgetManager::GetInst()->FindWidget<UPlayerStatusHUDWidget>(TEXT("PlayerStatusHUDWidget"));
			if (HUDWidget)
			{
				//프로그레션 -> 현재누적경험치-이전레벨최대경험치  vs  다음레벨경험치
				//이전레벨 최대경험치 = 다음레벨시작경험치 - 다음레벨까지필요경험치
				int32 PrevLevelMaxExp = ExpInfo->TotalExp - ExpInfo->RequiredExp;
				int32 AdjustedExp = mExp - PrevLevelMaxExp;
	
				HUDWidget->SetXPBar(AdjustedExp, ExpInfo->RequiredExp);
			}
		}
	}
}

void APlayerCharacter::LevelUp()
{
	AddLevel(1);
	//레벨업시 효과 구현
	//레벨업 보너스는 아직 미설계

	//이펙트 처리 
	///Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/SharedGameplay/States/LevelUp/P_LevelUp.P_LevelUp'
	UParticleSystem* LvlUpVFX= LoadObject<UParticleSystem>(
		GetWorld(),
		TEXT("Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/SharedGameplay/States/LevelUp/P_LevelUp.P_LevelUp'"));

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		LvlUpVFX,
		GetMesh()->GetComponentLocation(),
		FRotator::ZeroRotator);

	//사운드처리
	///Script/Engine.SoundWave'/Game/Sounds/sfx/piglevelwin2mp3-14800.piglevelwin2mp3-14800'

	//사운드생성 : 동기화방식- 런타임 로드
	USoundBase* LvlUpSFX = LoadObject<USoundBase>(
		GetWorld(),
		TEXT("Script/Engine.SoundWave'/Game/Sounds/sfx/piglevelwin2mp3-14800.piglevelwin2mp3-14800'"));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), LvlUpSFX,
		GetMesh()->GetComponentLocation(), FRotator::ZeroRotator,
		1.f);
}

void APlayerCharacter::GenerateRandomInvItem()
{
	UItemDataCacheManager* DataManager = UItemDataCacheManager::GetInst();
	if (DataManager)
	{
		//아이템 랜덤으로 하나 고르기 
		FName RandomItemNamePicked = DataManager->GetRandomItemRowName();

		//아이템 생성
		mInventoryItemsComponent->AddItem(RandomItemNamePicked);
	}
}

