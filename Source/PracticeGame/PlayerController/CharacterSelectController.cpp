// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectController.h"
#include "../UI/CharacterSelectWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "../Interface/CharacterRenderingInterface.h"
#include "../Player/PlayerCharacter.h"
#include "../Player/SelectPawn.h"

ACharacterSelectController::ACharacterSelectController()
{
	//IMC �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputMappingContext>
		IMC(
			TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_TestGame.IMC_TestGame'")
		);
	if (IMC.Succeeded())
		mIMC = IMC.Object;
	
	//�Է¾׼� �ε� �� �Ҵ�
	static ConstructorHelpers::FObjectFinder<UInputAction> IASelectChar(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SelectCharacter.IA_SelectCharacter'"));
	if (IASelectChar.Succeeded())
		mIASelectCharacter = IASelectChar.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> IADeselectChar(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_DeselectCharacter.IA_DeselectCharacter'"));
	if (IADeselectChar.Succeeded())
		mIADeselectCharacter = IADeselectChar.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget>
		SelectWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_CharacterSelect.UI_CharacterSelect_C'"));
	if (SelectWidgetClass.Succeeded())
		mSelectWidgetClass = SelectWidgetClass.Class;
}

void ACharacterSelectController::BeginPlay()
{
	Super::BeginPlay();

	//�������ؽ�Ʈ �ҷ�����
	ACharacterSelectController* PlayerController =
		Cast<ACharacterSelectController>(GetWorld()->GetFirstPlayerController());

	//ĳ���ÿ������ϸ�
	if (IsValid(PlayerController))
	{
		//���� �÷��̾��� ���� ����ý����� �����´� 
		//��, ���� �÷��̾ ����ϰ� �ִ� ���� �Է� ó�� ����ý����� �����´�
		UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::
			GetSubsystem<UEnhancedInputLocalPlayerSubsystem>
			(PlayerController->GetLocalPlayer());

		if (Subsystem)
		{
			UE_LOG(LogTemp, Warning, TEXT("Enhanced Input Subsystem found!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Enhanced Input Subsystem is NULL!"));
		}

		//CDO ���� APlayerControllerTestGame �� �����´�
		const ACharacterSelectController* CDO = GetDefault<ACharacterSelectController>();

		if (!Subsystem)
		{
			UE_LOG(LogTemp, Error, TEXT("Subsystem is NULL! Check LocalPlayer"));
		}


		//���� ���ؽ�Ʈ�� ���� �÷��̾��� ����ý��ۿ� �߰��Ѵ�
		Subsystem->AddMappingContext(CDO->mIMC, 1);
	}
	else
	{
		UE_LOG(TestGame, Display, TEXT("Invalid Player Controller"));
	}

	FInputModeGameAndUI InputMode;

	SetInputMode(InputMode);

	SetShowMouseCursor(true);



	// �о�� ���� Ŭ������ �̿��ؼ� ���� ��� ������ �����ϰ� ����Ʈ�� �����ش�.
	if (IsValid(mSelectWidgetClass))
	{
		mSelectWidget = CreateWidget<UCharacterSelectWidget>(GetWorld(), mSelectWidgetClass);

		if (IsValid(mSelectWidget))
			mSelectWidget->AddToViewport();
	}

	SetSelectionCamera();
}

void ACharacterSelectController::SetSelectionCamera()
{
	TArray<AActor*> FoundCameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundCameras);

	if (FoundCameras.Num() > 0)
	{
		ACameraActor* SelectionCamera = Cast<ACameraActor>(FoundCameras[0]); // ù ��° ī�޶� ����
		if (SelectionCamera)
		{
			SetViewTargetWithBlend(SelectionCamera, 0.5f); // �ε巯�� ��ȯ
		}
	}
}

void ACharacterSelectController::SelectActor()
{
	UE_LOG(TestGame, Display, TEXT("A Character has Selected"));
	FHitResult Hit;
	bool IsHit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, false, Hit);


	if (IsHit)
	{
		ASelectPawn* Target = Cast<ASelectPawn>(Hit.GetActor());

		//Ŭ������ �� ����� ��ȿ�� Ÿ���ΰ�� �ƿ������� ����Ѵ�
		if (Target)
		{
			//���� �ε��� ���������� �������
			if (mCurrentSelectedActor && mCurrentSelectedActor != Target)
			{
				mCurrentSelectedActor->EnableMeshStencil(false);		// ���� ���ٽ� ��ü ����
				mCurrentSelectedActor->ChangeAnim(ESelectAnim::Idle);	//���� ���� �ִϸ��̼� ���̵� ����  (��� ��� �ٵǸ� ��Ƽ���̸� �����ص����� �ڵ����� ���ư��� ��
				Target->EnableMeshStencil(true);				// �ƿ����� ����
				Target->ChangeAnim(ESelectAnim::Select);		// �ִϸ��̼� �÷��� ����
				mCurrentSelectedActor = Target;					// ���õ� ���� ����
				UE_LOG(TestGame, Display, TEXT("Clear and Replace SelectedActor"));
			}
			else
			{	//������ ���õ� ���Ͱ� ���� ��� ���Ӱ� �����Ѵ�
				Target->EnableMeshStencil(true);				 // �ƿ����� ����
				Target->ChangeAnim(ESelectAnim::Select);		// �ִϸ��̼� �÷��� ����
				mCurrentSelectedActor = Target;					// ���õ� ���� ����
				UE_LOG(TestGame, Display, TEXT("Bind New SelectedActor Info"));
			}

			//ī�޶� �̵� -> (X=145.000000,Y=0.000000,Z=45.000000)    (Pitch=-13.000000,Yaw=180.000000,Roll=0.000000)
			AActor* SelectedActor = Cast<AActor>(Hit.GetActor());
			if(SelectedActor)
				MoveCameraToSelectedActor(SelectedActor);

			//������ �ش� Ŭ���� ������ ������Ʈ�Ѵ�
			if (IsValid(mSelectWidget))
			{
				mSelectWidget->EnableStartButton(mCurrentSelectedActor != nullptr);
				mSelectWidget->EnableInfoWidget(mCurrentSelectedActor != nullptr);

				if (mCurrentSelectedActor)
				{
					mSelectWidget->SetCharacterImage(mCurrentSelectedActor->GetImageMaterial());

					ICharacterPublicInterface* Info = Cast<ICharacterPublicInterface>(mCurrentSelectedActor);

					mSelectWidget->SetCharacterInfo(Info);

					mSelectWidget->SetSelectedJob(mCurrentSelectedActor->GetJob());
				}
			}
		}
	}
}

void ACharacterSelectController::DeselectActor()
{
	//������ ���õ� ���Ͱ� ������ �ƿ������� �����ϰ� �ּҸ� �ʱ�ȭ�Ѵ�
	if (mCurrentSelectedActor)
	{
		//���� ���� ���Ϳ� ȣ���� ���Ͱ� ������ ȣ���嵵 ���� �����Ѵ� (ȣ���� üũ���� ����ó���Ǿ ����� ���� �ȵǰ� �ֱ� ����)
		if (mCurrentSelectedActor == mHoveredActor)
		{
			UE_LOG(TestGame, Warning, TEXT("Deselect HoveredActor"));
			mHoveredActor->EnableMeshStencil(false);
			mHoveredActor = nullptr;
		}
		mCurrentSelectedActor->EnableMeshStencil(false);		// �ƿ����� ����
		mCurrentSelectedActor->ChangeAnim(ESelectAnim::Idle);	//���̵� �ִ����� ����
		mCurrentSelectedActor = nullptr; // ���� ����
		UE_LOG(TestGame, Warning, TEXT("Deselect Previous Selected Actor"));

	}
	mSelectWidget->EnableInfoWidget(false);
	//ī�޶� �̵� -> �⺻ ��  (X=-800.000000,Y=-60.000000,Z=180.000000)   (Pitch=6.000000,Yaw=0.000000,Roll=0.000000)
	MoveCameraToDefault();
}

void ACharacterSelectController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//���콺 �����ͷκ��� �����ϴ� 3���� ��ü�� �浹�Ͽ� ��������� ȹ���Ѵ�
	FHitResult Hit;
	bool Pick = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, true, Hit);		//MouseTrace ä�η� �����Ͽ���

	//�浹�� �߻��� ���
	if (Pick)
	{
		//�� �������̽��� ����ϴ� ��� ���ʹ� Ÿ�� ���⿡ ����ϰ� �� ���̴� 
		//Ÿ���� Ŭ���� ������ �������� �̷����� ó���� ���� Ÿ�� �˻纸�� �����ϴ�
		ASelectPawn* Target = Cast<ASelectPawn>(Hit.GetActor());

		//���� �ε��� ���������� �ְ� ���õ� ���Ͱ� �ƴѰ��
		if (mHoveredActor && mHoveredActor != mCurrentSelectedActor)
		{
			//������ ���Ͱ� ���� ���Ͱ� �ƴѰ�� ������ ȣ����� �ƿ����� ���� ó��
			if (mHoveredActor != Target)
			{
				mHoveredActor->EnableMeshStencil(false);
			}
		}

		//��ȿ�� Ÿ��� �浹�� ���
		if (Target)
		{
			Target->EnableMeshStencil(true);
		}

		mHoveredActor = Target;
	}

	//�浹�� �߻����� ���� ���  -> ������ �ƿ����� ���� 
	else
	{
		//UE_LOG(TestGame, Display, TEXT("Nothing Collided"));
		//������ �ƿ����� �����ϸ� ����, �� ���õ� ���Ͱ� �ƴ� ��쿡��
		if (mHoveredActor  && mHoveredActor != mCurrentSelectedActor)
		{
			mHoveredActor->EnableMeshStencil(false);
			mHoveredActor = nullptr;
		}
	}
}

void ACharacterSelectController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//���°� Ȯ�οϷ�

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))	//���� üũ �Ϸ�
	{
		// üũ �Ϸ�   UE_LOG(LogTemp, Warning, TEXT("mIMC is %s"), mIMC ? TEXT("Valid") : TEXT("NULL"));				
		// üũ �Ϸ�   UE_LOG(LogTemp, Warning, TEXT("mIASelectCharacter is %s"), mIASelectCharacter ? TEXT("Valid") : TEXT("NULL"));
		// üũ �Ϸ�   UE_LOG(LogTemp, Warning, TEXT("mIADeselectCharacter is %s"), mIADeselectCharacter ? TEXT("Valid") : TEXT("NULL"));

		//��Ŭ�� :: ���� ����
		EnhancedInputComponent->BindAction(mIASelectCharacter, ETriggerEvent::Started, this, &ACharacterSelectController::SelectActor);
		//��Ŭ�� :: ���� ���� ����
		EnhancedInputComponent->BindAction(mIADeselectCharacter, ETriggerEvent::Started, this, &ACharacterSelectController::DeselectActor);
	}
}

void ACharacterSelectController::MoveCameraToSelectedActor(AActor* SelectedActor)
{
	if (!mCurrentSelectedActor)
	{
		UE_LOG(TestGame, Warning, TEXT("No selected actor to move camera."));
		return;
	}

	// �ش� �������� SelectCameraActor ã��
	AActor* CameraActor = nullptr;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->IsA(ACameraActor::StaticClass()))
		{
			CameraActor = *It;
			break;
		}
	}

	if (!CameraActor)
	{
		UE_LOG(TestGame, Error, TEXT("SelectCameraActor not found."));
		return;
	}


	// ���õ� ������ ���� ��ġ ��������
	FVector TargetLocation = SelectedActor->GetActorLocation();
	FRotator TargetRotation = SelectedActor->GetActorRotation();

	// ���� ������ �������� ��ġ ����
	FVector ForwardOffset = TargetRotation.Vector() * 185.0f; // �������� 145 ���� �̵�
	FVector NewLocation = TargetLocation + ForwardOffset + FVector(0.0f, 0.0f, 45.0f);

	// ȸ�� ���� ����
	FRotator NewRotation = TargetRotation + FRotator(-13.0f, 0.0f, 0.0f); // Pitch�� ����
	NewRotation.Yaw += 180.0f;

	CameraActor->SetActorLocation(NewLocation);
	CameraActor->SetActorRotation(NewRotation);
	

	SetViewTargetWithBlend(CameraActor, 1.0f); // �ε巯�� ��ȯ

	UE_LOG(TestGame, Display, TEXT("Camera moved to world position of selected actor."));
}

void ACharacterSelectController::MoveCameraToDefault()
{
	// �ش� �������� SelectCameraActor ã��
	AActor* CameraActor = nullptr;
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		if (It->IsA(ACameraActor::StaticClass()))
		{
			CameraActor = *It;
			break;
		}
	}

	if (!CameraActor)
	{
		UE_LOG(TestGame, Error, TEXT("SelectCameraActor not found."));
		return;
	}
	// ��ǥ ��ġ/ȸ��
	// (X=-800.000000,Y=-60.000000,Z=180.000000)   (Pitch=6.000000,Yaw=0.000000,Roll=0.000000)
	FVector DefaultLocation = FVector(-879.0f, -60.0f, 180.0f);
	FRotator DefaultRotation = FRotator(6.0f, 0.0f, 0.0f);

	CameraActor->SetActorLocation(DefaultLocation);
	CameraActor->SetActorRotation(DefaultRotation);

	SetViewTargetWithBlend(CameraActor, 1.0f); // �ε巯�� ��ȯ
}
