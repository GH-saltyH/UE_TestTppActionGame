// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "../Player/Animation/AnimInstancePlayer.h"
//�α� ī�װ� ����
DEFINE_LOG_CATEGORY(PlayerLog);

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//�ν��Ͻ��� �����ϰ� �ε��Ѵ�. 
	// TEXT("���") �� ���ϴ� ������ ��θ� �����Ѵ�
	//SkeletalMesh ��ü�� �ε��ߴ�. 
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> 
		SkeletalMesh(TEXT(
			"/Script/Engine.SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));
	// �ν��Ͻ� �ε忡 �����ϸ� ĳ���� Ŭ������ �޽�������Ʈ��
	// �ε�� �޽��� �����Ѵ�.
	if (SkeletalMesh.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(SkeletalMesh.Object);

	//���̷�Ż�޽� Z = -90.f
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.5f));
	//���̷�Ż�޽��� Yaw = -90.f
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	//ĸ�� ���� ���� = 90.5
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.5f);

	//�÷��̾� Yaw 180.f
	SetActorRelativeRotation(FRotator(0, 180, 0));
	

	/*
		���������Ʈ : ������Ʈ�� �߰�
		�������ϰ� ī�޶� ������Ʈ�� �߰��Ѵ�
	*/
	mSpringArm = CreateDefaultSubobject
		<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject
		<UCameraComponent>(TEXT("Camera"));

	/*
		���������� ��Ʈ ������Ʈ�� �θ�� �Ѵ�
		ī�޶�� ���������� �θ�� �Ѵ�
	*/

	mSpringArm->SetupAttachment(RootComponent);
	mCamera->SetupAttachment(mSpringArm);

	/* 
		���������� Ÿ�� �� ���̸� 300.f �� �����Ѵ�
		���������� Pitch  �� -20 ��ŭ ��������� ȸ���Ѵ�
		���������� Z ������ġ�� +39.0f ��ŭ ��������� �̵��Ѵ�
		*/
	mSpringArm->TargetArmLength = 300.f;
	mSpringArm->SetRelativeRotation(FRotator(-20, 0, 0));
	mSpringArm->SetRelativeLocation(FVector(0, 0, 39));

	// mCamera->AttachToComponent(mSpringArm); 
	// AttachToComponent �� �ش� ������Ʈ�� USceneComponent �� ������Ų��
	
		/*
	1. UClass ������ ��������
		: APlayerCharacter �� UClass ������ �����´�
				UClass �� ���÷��� �ý����� ���� �ش� Ŭ������ 
				�⺻ ������ �����ϴ� ����ü�̴� */
	//UClass* MyClass = APlayerCharacter::StaticClass();
	// ���� MyClass �� APlayerCharacter Ŭ������ UClass ������ �����Ѵ�

	/*
	2. CDO ��������
		: GetDafaultObject<ACharacter>()  ��   Class Default Object �� ��ȯ�Ѵ�
		��ȯ�� CDO �� �� Ŭ������ �⺻ �ν��Ͻ��̴�.
		ACharacter Ŭ������ ��� �ν��Ͻ��� �� ��ü�� ������� �����ȴٴ� �ǹ��̴�
	*/
	//ACharacter* DefaultCharacter = MyClass->GetDefaultObject();
	//���� DefaultCharacter ��  CPlayerCharacter Ŭ������ �⺻ ��ü�� �ȴ�.

	//ĳ���͹����Ʈ ������Ʈ�� ���´�
	//GetCharacterMovement �� 
	// UCharacterMovementComponent* �� ��ȯ�Ѵ�
	
	//JumpZVelocity ���� ���̸� �����Ѵ�
	GetCharacterMovement()
		->JumpZVelocity = 600.f;	
	//GravityScale  �߷��� �����Ѵ�
	GetCharacterMovement()
		->GravityScale = 1.0f; 
	//AirControl  ���� �̵� ��� �����Ѵ�, 
	// ���� ���� �̵��� ���ѵǰ�, �������� ���� �̵��� �����ο����� 
	GetCharacterMovement()
		->AirControl = 0.5f;

	//JumpMaxCount ��  �����ϱ� ������ �ִ� ������ ���� Ƚ���� �����Ѵ�
	JumpMaxCount = 2;

	/*
		�ִϸ��̼� �������Ʈ ���� �ε�
		�ִ��ν��Ͻ��� ��ӹް� �ִ� TSubClassOf<UAnimInstance> Ÿ���� Ŭ�����̴�
		FClassFinder �� ����ؾ� �ϸ�, 
		�������Ʈ�̱� ������ ��� �������� _C'  �� �ٿ��� �Ѵ� 	*/
	static ConstructorHelpers::FClassFinder<UAnimInstance> 
		ABPCharacter(
			TEXT(
				"/Script/Engine.AnimBlueprint'/Game/Player/Animation/ABP/ABPCharacter.ABPCharacter_C'")
		);

	//���� �ε忡 �����ϸ�  ���� ���̷�Ż �޽���  �ִ��ν��Ͻ� Ŭ������ �����Ѵ�
	if (ABPCharacter.Succeeded())
		GetMesh()->SetAnimInstanceClass(ABPCharacter.Class);

	//����ü ���·ε�
	static ConstructorHelpers::FClassFinder<AATestProjectile> TestProjectile(
		TEXT(
			"/Script/CoreUObject.Class'/Script/PracticeGame.ATestProjectile'")
	);
	//���� �ּ� �Ҵ�
	if (TestProjectile.Succeeded())
		mProjectileClass = TestProjectile.Class;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//���ǵ� �÷��̾� ��Ʈ�ѷ��� ���´�
	//GetController �� AController Ÿ���� ��ȯ�ϱ� ������ ĳ������ �ʿ��ϴ�
	//UObject �� ��ӹް� �־� Cast �� �Ἥ �����ϰ� ����ȯ�� �� �� �ִ�

	APlayerController* PlayerController = 
		Cast<APlayerController>(GetController());

	//ĳ���ÿ������ϸ�
	if (IsValid(PlayerController))
	{
		//���� �÷��̾��� ���� ����ý����� �����´� 
		//��, ���� �÷��̾ ����ϰ� �ִ� ���� �Է� ó�� ����ý����� �����´�
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::
			GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(PlayerController->GetLocalPlayer());

		//CDO ���� APlayerControllerTestGame �� �����´�
		const APlayerControllerTestGame* APCTestGameCDO = GetDefault<APlayerControllerTestGame>();

		//���� ���ؽ�Ʈ�� ���� �÷��̾��� ����ý��ۿ� �߰��Ѵ�
		Subsystem->AddMappingContext(APCTestGameCDO->mIMC, 0);
	}

	//�ִ��ν��Ͻ��� �޾ƿ´�
	mAnimInst = Cast<UAnimInstancePlayer>(GetMesh()->GetAnimInstance());
}

/*BindAction ���� ���ε� �Ǿ� ������,
�Է¾׼� 'IAMove' �� Triggered �� �� ȣ��Ǵ� �Լ��̴�.*/
void APlayerCharacter::OnIAMoveTriggered(const FInputActionValue& Value)
{
	//�̵� ������ ���� ����
	if (!IsCharacterImmobilized())
	{
		/*
		���ڷ� �޾ƿ��� ���� �Է� �׼� ������ ���� ���ؽ�Ʈ���� ������ ������ ������
			����
				IA_Move �� 2D �������� �����ϰ� �ִ�
				�������ؽ�Ʈ������
					Ű���� �Է��� X ��� Y ��(������ �� ���) ���� �����Ͽ���
				Triggered ��� 1 (���� �� ��� -1)
							 �ƴϸ� 0 �̴�*/

							 //�Է°��� ���ͷ� ��ȯ�� ���� ������ ������ �����Ѵ�
							 //���� ����� ���� ���� ���̴�.
		FVector Axis;

		//�Է¿��� ���޵� ���� ���ͷ� �����Ѵ�.
		//Value ���� ���� ó���� ���� switch ���� ���·� �����ߴ�
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

		// ������ ����ϱ����� FVector ��ü�̴�. 0���� �ʱ�ȭ�Ѵ�
		FVector Direction = FVector::Zero();

		//X ���� ���� �޾� ���翢���� ���溤�� (X��) �� ���Ѵ�
		//Y ���� ���� �޾� ���� ������ �������� (Y��) �� ���Ͽ� ��� ���� �����Ѵ�
		Direction = GetActorForwardVector() * Axis.X;
		Direction += GetActorRightVector() * Axis.Y;

		/* �Ǵ�
		 Direction = FVector(GetActorForwardVector() * Axis.X,
										GetActorRightVector() * Axis.Y,
										GetActorUpVector());							*/

										//����ȭ�� �Ͽ� �밢���� �Է��� �Ų����� ó���Ѵ�.
										//���� ó�� 0 ����ȭ�� �����Ѵ�
		if (!Direction.IsNearlyZero())
		{
			//���� ó�� �ε��Ҽ��� �������(���� ���� �� ��) �� �����Ѵ�
			const float Threshold = KINDA_SMALL_NUMBER;
			if (Direction.SizeSquared() > Threshold)
			{
				Direction.Normalize();
				//���� ó�� ����ȭ �� ũ�� ����
				Direction *= 1.0f / FMath::Max(Direction.Size(), 1.0f);
				if (bDebugEnabled)
					UE_LOG(PlayerLog, Display, TEXT("Direction Normalized Safety : ( %.1f, %.1f, %.1f )"), Direction.X, Direction.Y, Direction.Z);
			}
		}

		//���� �÷��̾� �޽��� �ִ��ν��Ͻ��� �޾� 
		// UAnimInstancePlayer �� ĳ�����Ѵ�
		UAnimInstancePlayer* AnimInst =
			Cast<UAnimInstancePlayer>(GetMesh()->GetAnimInstance());

		//ĳ���ÿ� �����ϸ� 
		//Axis ��
		//		: �Է����� ���� X �� Y �� ��  FVector �� ��ȯ �� ��
		//�ִ��ν��Ͻ��� mDirection ����� �Ҵ��Ѵ�
		if (IsValid(AnimInst))
			AnimInst->SetDirection(Axis);

		//��� �Ϸ�� ���ͷ� �Ʒ��� �Լ��� ȣ���Ѵ�
		//MoveToAxis �� �̵��� ó���ϸ� �̵��� ���õ� �߰� ����� ������ �� �ֵ��� 
		//����� ���Ƿ� �ۼ��� ��� �Լ��̴�
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

//�� �Լ��� ���� �Լ��� 
// IA_Jump �� ������ Ű�� Started �Ǿ����� ȣ��ȴ�.
void APlayerCharacter::OnIAJumpStarted(const FInputActionValue& Value)
{
	//CanJump �� ������ ������ ���������� ��ȯ�Ѵ�
	//������ �����ϰ� �̵��Ұ��� �ƴ� ���� �����Ѵ�
	if (CanJump() && !IsCharacterImmobilized())
	{
		GetCharacterMovement()->bNotifyApex = true; // �ְ��� �˸� Ȱ��ȭ
		//mCharacterState.ClearState(mCharacterState.ACTING);
		//Jump �� ĳ���Ͱ� ���� �ϵ��� �ϴ� ��� �Լ��̴�.
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
	//������ ������ ���� ������ �Ѵ� ����. ����/�ٸ�����/����������������� �� 
	if (CanCharacterAct())
	{
		//���� ���� ���� �� �ɸ� �� �ִ� ���� ���Ѱ�ӵ� ������ �ӽ� �ذ� (�߻� ���ɼ� ����)
		mCharacterState.ClearState(mCharacterState.LANDING);

		if (mAnimInst)
		{
			//���� ��� �ִ� �ִ��ν��Ͻ����� ���ݸ�Ÿ�ָ� �޾ƿ��� �����ϸ� ����
			UAnimMontage* AttackMontage = mAnimInst->GetAttakMontage();
			if (AttackMontage)
			{
				//�÷��׸� �����Ѵ� ACTING : ON  -- ���������� ���� ������ ���Ѵ�
				mCharacterState.SetState(mCharacterState.ACTING);

				//�޺� ������ ��� �ε��� ��ȯ ó��
				if (mCharacterState.HasState(mCharacterState.COMBO))
				{
					//���� ��Ÿ�� ������ �ε����� ��ȯ �����Ѵ�
					mAnimInst->mSectionIndex 
						= (mAnimInst->mSectionIndex + 1) % mAnimInst->mAttackSectionArray.Num();
					//���� ����� ��Ÿ�� �̸��Ҵ�(����� ��¿�)
					//FName NextSectionName = mAnimInst->mAttackSectionArray[mAnimInst->mSectionIndex];
				}
				//�޺��� �ƴѰ�� �ε��� �ʱ�ȭ
				else
				{
					mAnimInst->mSectionIndex = 0;
				}

				//���� �ε����� Ȱ���Ͽ� ����Ϸ��� ���ݼ����� �̸��� �޾ƿ´�
				FName SectionName = mAnimInst->mAttackSectionArray[mAnimInst->mSectionIndex];
				
				//Ŀ�����Լ�: ���� ��Ÿ�ֿ��� 
				//���� �Ҵ�� ������ �̸��� ����Ͽ� �� ������ �ٷ� �����Ų�� 
				mAnimInst->PlayMontageSection(AttackMontage, SectionName);
				
				//�������� ���� ��� ��Ÿ�� �̸��� �ε��� 
				UE_LOG(TestGame, Display,
					TEXT("Current Playing Montage [%s],\tIndex[%d]"),
					*SectionName.ToString(),
					mAnimInst->mSectionIndex);

				//���� ������ ��ġ�� ȸ������ ���߱� ���� ���� ������
				FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
				FRotator SpawnRotation = GetActorRotation();

				//AATestProjectile Ŭ������ ����ü�� ������
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
	//ȸ�� �� FRotator ���
	//�Է°����� X �� Y ���� �����´�
	FVector2D Input = Value.Get<FVector2D>();
		/* �Է°� �� ����
			1. ���콺 �̵��ϱ� (���ӵ��� �����ϴ� �Ǽ�)
				�����̵�   -X 
				�����̵�	+X 
				����̵�	-Y
				�Ϲ��̵�	+Y
			2. �����е� ������ƽ ����̱� ( 0 ~ 1  �Ǽ�,  ������ ��� -1 ~ 0)
				�������α���̱�		-X
				�������α���̱�		+X
				���ο��̱�			+Y
				�Ʒ��α���̱�		-Y				*/

	//ȸ���ӵ��� ���� �����Ѵ�
	float RotationSpeed = 2.0f;
	//����ȸ������ �Ѱ踦 �����Ѵ�
	float MaxPitchLimit = 75.0f;
	//���Ʋ�� ������: ���������� 90���� �ʰ��ϴ� �ʹ� ū �� ó��
	Input.Y = (85 - MaxPitchLimit) < RotationSpeed * Input.Y 
		? (85 - MaxPitchLimit) / RotationSpeed 
		: Input.Y;
	Input.Y = (MaxPitchLimit - 85) > RotationSpeed * Input.Y
		? (MaxPitchLimit - 85) / RotationSpeed
		: Input.Y;

	//�������� ���縦 üũ�ϰ���
	//�Է¹��� �� �ప(float)�� 
	// ���������� ��� ȸ���� �� ��ҿ� �����Ѵ�
	// ** Ʈ������ â ������  Roll, Yaw, Pitch
	
	if (mSpringArm)
	{
		//���Ϸ� ȸ��
		//// ���� ���������� ȸ������ �����´�
		//FRotator NewRotation = mSpringArm->GetRelativeRotation();
		//
		//// ���Ʒ� �Է½�   Pitch �� ���� +,  �Ʒ��� �Է½�  -
		//NewRotation.Pitch += Input.Y * RotationSpeed; 
		//// �¿� �Է� �� Yaw�� ����, ��- ��+
		//NewRotation.Yaw += Input.X * RotationSpeed;
		//
		//***���Ϸ� ȸ���� ���� ��ȭ 
		//1. Pitch ���� ����
		//if (FMath::Abs(NewRotation.Pitch) >= 85.0f)
		//	NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch, -85.0f, 85.0f);
		////ȸ�� ���� �����Ѵ�
		//mSpringArm->SetRelativeRotation(NewRotation);

		//2. ���ʹϾ� Ȱ��
		FQuat CurrentQuat = mSpringArm->GetRelativeRotation().Quaternion();

		// �Է°��� ������� ȸ�� ��ȯ ���� (Yaw ����, Pitch ����)
		FQuat PitchQuat = FQuat(FVector::RightVector, FMath::DegreesToRadians(-Input.Y * RotationSpeed));
		FQuat YawQuat = FQuat(FVector::UpVector, FMath::DegreesToRadians(Input.X * RotationSpeed));
	
		// ���ο� ȸ�� ����
		//FQuat NewQuat = YawQuat * CurrentQuat * PitchQuat;    //�⺻ ���
		// Pitch ���� ��� : Yaw �� �����صд�
		FQuat NewQuat = CurrentQuat * PitchQuat;	
		
		// PitchQuat * YawQuat * CurrentQuat;
		// ȸ�� ������ Pitch 90 ��������  Y �� Z �� ��Ʋ���¹����� �ذ��Ѵ�. 
		// ������ �ش������� �����ϸ�  Y ȸ���ϸ鼭 ��ġ�� ���������� �ǵ��ư��� ���¸� ���
		//    FQuat NewQuat = PitchQuat * YawQuat * CurrentQuat;

		NewQuat.Normalize();

		// ���� Pitch ���� ���ʹϾ𿡼� ���� ������ ������ ����
		FVector ForwardVector = NewQuat.GetForwardVector();
		FRotator RotationFromQuat = ForwardVector.Rotation();
		//���� ���� : ���� ī�޶�ȸ���� �Ʒ��� ���� �����Ѵ�
		RotationFromQuat.Pitch = FMath::Clamp(RotationFromQuat.Pitch, -MaxPitchLimit, MaxPitchLimit);

		// ���ѵ� Pitch�� �ݿ��Ͽ� ���ʹϾ� ���� �� Yaw �� ����
		FQuat ClampedQuat = FQuat::MakeFromRotator(RotationFromQuat);
		ClampedQuat.Normalize();

		FQuat FinalQuat = YawQuat * ClampedQuat;
		mSpringArm->SetRelativeRotation(FinalQuat);

		////���� Pitch ��ȭ�� ���� ���� ��Ÿ�� �Ѱ踸���
		//// Pitch �� ��ȭ�� ������ �����Ѵ�
		//float PitchDelta = RotationFromQuat.Pitch - PrevPitch;
		//PitchDelta = FMath::Clamp(PitchDelta, -MaxPitchChange, MaxPitchChange);
		//RotationFromQuat.Pitch = PrevPitch + PitchDelta;

		//FQuat ClampedQuat = FQuat::MakeFromRotator(RotationFromQuat);
		//ClampedQuat.Normalize();

		//// ���������� ����
		//mSpringArm->SetRelativeRotation(ClampedQuat);

		//// ���� Pitch �� ����
		//PrevPitch = RotationFromQuat.Pitch;

		//3. Roll �� ����
		//Roll �� ������ �ʱ�ȭ�Ͽ� �����Ѵ�
		//mSpringArm->SetRelativeRotation(FRotator(NewRotation.Pitch, NewRotation.Yaw, 0));
		
		//�ִ��ν��Ͻ����� �� ������ �ǳ��ش�
		// ������ �ذ��� 1. �������� Y Ŭ���� �� �� ���޹��
		//mAnimInst->SetCamRotateByInput(NewRotation);
		// ������ �ذ��� 2. ���ʹϾ� ȸ��  �� �� ���޹��
		mAnimInst->SetCamRotateByInput(FinalQuat.Rotator());
		
		//����� ��¿�
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

			//�����
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
	//�ϳ��� true �� �̵��Ҵ� �����̴�
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
		//�ϳ��� true �� �ൿ�Ҵ� �����̴�
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

	//������ PlayerInputComponent �� 
	// APlayerController Ŭ������ InputComponent* �̴�
	// �� ������Ʈ�� �����Է�������Ʈ* �� ĳ������ ��ü�� �����Ѵ�.
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	//ĳ���ÿ� ������
	if (IsValid(EnhancedInputComponent))
	{
		//CDO ���� APlayerControllerTestGame �� �����´�
		const APlayerControllerTestGame* APCTestGameCDO = GetDefault<APlayerControllerTestGame>();

		//IA_Move �Է¾׼� �������� �Լ��� ���ε��Ѵ� 
		// �̺�Ʈ�� Triggered �������� ȣ���Ѵ�.
		//		--> �÷��̾�ĳ������ ����Լ� OnIAMoveTriggered �� ���ε��ߴ�
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIAMove,
				ETriggerEvent::Triggered,
				this,
				&APlayerCharacter::OnIAMoveTriggered);

		//IA_Jump �Է¾׼� �������� �Լ��� ���ε��Ѵ� 
		// �̺�Ʈ�� Started �������� ȣ���Ѵ�. ������ �־ ù �Է� �ÿ��� ȣ���Ѵ�
		//		--> �÷��̾�ĳ������ ����Լ� OnIAJumpStarted �� ���ε��ߴ�
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIAJump,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::OnIAJumpStarted);

		//IA_Attack �Է¾׼� �������� �Լ��� ���ε��Ѵ� 
		// �̺�Ʈ�� Started �������� ȣ���Ѵ�. ������ �־ ù �Է� �ÿ��� ȣ���Ѵ�
		//		--> �÷��̾�ĳ������ ����Լ� OnIAAttackStarted �� ���ε��ߴ�
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIAAttack,
				ETriggerEvent::Started,
				this,
				&APlayerCharacter::OnIAAttackStarted);

		//IA_Attack �Է¾׼� �������� �Լ��� ���ε��Ѵ� 
		// �̺�Ʈ�� Triggered �������� ȣ���Ѵ�.
		//		--> �÷��̾�ĳ������ ����Լ� OnIAAttackTriggered �� ���ε��ߴ�
		EnhancedInputComponent->
			BindAction(APCTestGameCDO->
				mIAAttack,
				ETriggerEvent::Triggered,
				this,
				&APlayerCharacter::OnIAAttackTriggered);

		//IA_CameraRotate �Է¾׼� �������� �Լ��� ���ε��Ѵ� 
		// �̺�Ʈ�� Triggered �������� ȣ���Ѵ�.
		//		--> �÷��̾�ĳ������ ����Լ� OnIACamRotateTriggered �� ���ε��ߴ�
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

		//�߷� ����
		float GravityAbs = FMath::Abs(GetCharacterMovement()->GetGravityZ());
		//�̷л� �ִ� ���� ���̸� ����Ѵ�
		float MaxJumpHeight 
			= (GetCharacterMovement()->JumpZVelocity * GetCharacterMovement()->JumpZVelocity)
			/ (2 * GravityAbs);
		//�̷л� �⺻ ������ ���� �ӵ� (������ 5%�߰�)
		float MaxLandingSpeed
			= sqrt(2 * GravityAbs * MaxJumpHeight) * 1.05f;
		//���� �ӵ��� �����´� (X Y �̵� �ӵ��� �����ϱ� ���� Z �ุ ����Ѵ�
		float CurrentVerticalSpeed
			= FMath::Abs(GetCharacterMovement()->GetLastUpdateVelocity().Z);


		//���� ������ �ƴ� ������� ���� ��Ÿ�� ���(��Ƽ���̱��� ��õ��� �̵��� �ൿ �Ҵ� ó��)
		if (CurrentVerticalSpeed > MaxLandingSpeed)
		{
			UE_LOG(PlayerLog, Display, TEXT("falling from higher apex than normal jump.\tSpeed %0.2f  > MAX[%0.2f]"), CurrentVerticalSpeed, MaxLandingSpeed);
			//�ɸ��� ���� �÷��׿� �̵��Ұ��� �����Ѵ� (LandEnd ��Ƽ���̿��� �����Ѵ�)
			mCharacterState.SetState(mCharacterState.LANDING);
			//���� �� ���� ������ ����Ű�� ���� �÷��� ��Ǯ�� ���� �ذ� 
			mCharacterState.SetState(mCharacterState.ACTING);
			UAnimMontage* LandMontage = mAnimInst->GetLandMontage();
			//Land��Ÿ�� ���
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
