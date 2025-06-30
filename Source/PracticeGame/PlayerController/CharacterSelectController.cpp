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
	//IMC 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputMappingContext>
		IMC(
			TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_TestGame.IMC_TestGame'")
		);
	if (IMC.Succeeded())
		mIMC = IMC.Object;
	
	//입력액션 로드 및 할당
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

	//매핑컨텍스트 불러오기
	ACharacterSelectController* PlayerController =
		Cast<ACharacterSelectController>(GetWorld()->GetFirstPlayerController());

	//캐스팅에성공하면
	if (IsValid(PlayerController))
	{
		//현재 플레이어의 로컬 서브시스템을 가져온다 
		//즉, 현재 플레이어가 사용하고 있는 향상된 입력 처리 서브시스템을 가져온다
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

		//CDO 에서 APlayerControllerTestGame 을 가져온다
		const ACharacterSelectController* CDO = GetDefault<ACharacterSelectController>();

		if (!Subsystem)
		{
			UE_LOG(LogTemp, Error, TEXT("Subsystem is NULL! Check LocalPlayer"));
		}


		//매핑 컨텍스트를 현재 플레이어의 서브시스템에 추가한다
		Subsystem->AddMappingContext(CDO->mIMC, 1);
	}
	else
	{
		UE_LOG(TestGame, Display, TEXT("Invalid Player Controller"));
	}

	FInputModeGameAndUI InputMode;

	SetInputMode(InputMode);

	SetShowMouseCursor(true);



	// 읽어온 위젯 클래스를 이용해서 실제 사용 위젯을 생성하고 뷰포트에 보여준다.
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
		ACameraActor* SelectionCamera = Cast<ACameraActor>(FoundCameras[0]); // 첫 번째 카메라 선택
		if (SelectionCamera)
		{
			SetViewTargetWithBlend(SelectionCamera, 0.5f); // 부드러운 전환
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

		//클릭했을 때 대상이 유효한 타깃인경우 아웃라인을 토글한다
		if (Target)
		{
			//기존 부딪힌 엑터정보가 있을경우
			if (mCurrentSelectedActor && mCurrentSelectedActor != Target)
			{
				mCurrentSelectedActor->EnableMeshStencil(false);		// 기존 스텐실 객체 해제
				mCurrentSelectedActor->ChangeAnim(ESelectAnim::Idle);	//기존 엑터 애니메이션 아이들 변경  (사실 재생 다되면 노티파이만 설정해뒀으면 자동으로 돌아가긴 함
				Target->EnableMeshStencil(true);				// 아웃라인 적용
				Target->ChangeAnim(ESelectAnim::Select);		// 애니메이션 플래그 변경
				mCurrentSelectedActor = Target;					// 선택된 엑터 저장
				UE_LOG(TestGame, Display, TEXT("Clear and Replace SelectedActor"));
			}
			else
			{	//기존의 선택된 엑터가 없는 경우 새롭게 적용한다
				Target->EnableMeshStencil(true);				 // 아웃라인 적용
				Target->ChangeAnim(ESelectAnim::Select);		// 애니메이션 플래그 변경
				mCurrentSelectedActor = Target;					// 선택된 엑터 저장
				UE_LOG(TestGame, Display, TEXT("Bind New SelectedActor Info"));
			}

			//카메라 이동 -> (X=145.000000,Y=0.000000,Z=45.000000)    (Pitch=-13.000000,Yaw=180.000000,Roll=0.000000)
			AActor* SelectedActor = Cast<AActor>(Hit.GetActor());
			if(SelectedActor)
				MoveCameraToSelectedActor(SelectedActor);

			//위젯에 해당 클래스 내용을 업데이트한다
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
	//기존의 선택된 엑터가 있으면 아웃라인을 제거하고 주소를 초기화한다
	if (mCurrentSelectedActor)
	{
		//기존 선택 엑터와 호버드 엑터가 같으면 호버드도 같이 삭제한다 (호버드 체크에서 예외처리되어서 제대로 제거 안되고 있기 때문)
		if (mCurrentSelectedActor == mHoveredActor)
		{
			UE_LOG(TestGame, Warning, TEXT("Deselect HoveredActor"));
			mHoveredActor->EnableMeshStencil(false);
			mHoveredActor = nullptr;
		}
		mCurrentSelectedActor->EnableMeshStencil(false);		// 아웃라인 제거
		mCurrentSelectedActor->ChangeAnim(ESelectAnim::Idle);	//아이들 애님으로 변경
		mCurrentSelectedActor = nullptr; // 선택 해제
		UE_LOG(TestGame, Warning, TEXT("Deselect Previous Selected Actor"));

	}
	mSelectWidget->EnableInfoWidget(false);
	//카메라 이동 -> 기본 값  (X=-800.000000,Y=-60.000000,Z=180.000000)   (Pitch=6.000000,Yaw=0.000000,Roll=0.000000)
	MoveCameraToDefault();
}

void ACharacterSelectController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//마우스 포인터로부터 존재하는 3차원 물체와 충돌하여 대상정보를 획득한다
	FHitResult Hit;
	bool Pick = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, true, Hit);		//MouseTrace 채널로 설정하였다

	//충돌이 발생한 경우
	if (Pick)
	{
		//이 인터페이스를 상속하는 모든 엑터는 타깃 검출에 통과하게 될 것이다 
		//타깃의 클래스 유형이 많을수록 이런식의 처리가 개별 타깃 검사보다 유리하다
		ASelectPawn* Target = Cast<ASelectPawn>(Hit.GetActor());

		//기존 부딪힌 엑터정보가 있고 선택된 엑터가 아닌경우
		if (mHoveredActor && mHoveredActor != mCurrentSelectedActor)
		{
			//기존의 엑터가 현재 엑터가 아닌경우 기존의 호버드는 아웃라인 제거 처리
			if (mHoveredActor != Target)
			{
				mHoveredActor->EnableMeshStencil(false);
			}
		}

		//유효한 타깃과 충돌한 경우
		if (Target)
		{
			Target->EnableMeshStencil(true);
		}

		mHoveredActor = Target;
	}

	//충돌이 발생하지 않은 경우  -> 기존의 아웃라인 제거 
	else
	{
		//UE_LOG(TestGame, Display, TEXT("Nothing Collided"));
		//기존의 아웃라인 존재하면 제거, 단 선택된 엑터가 아닌 경우에만
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

	//들어온거 확인완료

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))	//성공 체크 완료
	{
		// 체크 완료   UE_LOG(LogTemp, Warning, TEXT("mIMC is %s"), mIMC ? TEXT("Valid") : TEXT("NULL"));				
		// 체크 완료   UE_LOG(LogTemp, Warning, TEXT("mIASelectCharacter is %s"), mIASelectCharacter ? TEXT("Valid") : TEXT("NULL"));
		// 체크 완료   UE_LOG(LogTemp, Warning, TEXT("mIADeselectCharacter is %s"), mIADeselectCharacter ? TEXT("Valid") : TEXT("NULL"));

		//좌클릭 :: 엑터 선택
		EnhancedInputComponent->BindAction(mIASelectCharacter, ETriggerEvent::Started, this, &ACharacterSelectController::SelectActor);
		//우클릭 :: 엑터 선택 해제
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

	// 해당 레벨에서 SelectCameraActor 찾기
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


	// 선택된 엑터의 월드 위치 가져오기
	FVector TargetLocation = SelectedActor->GetActorLocation();
	FRotator TargetRotation = SelectedActor->GetActorRotation();

	// 전방 방향을 기준으로 위치 조정
	FVector ForwardOffset = TargetRotation.Vector() * 185.0f; // 전방으로 145 유닛 이동
	FVector NewLocation = TargetLocation + ForwardOffset + FVector(0.0f, 0.0f, 45.0f);

	// 회전 방향 유지
	FRotator NewRotation = TargetRotation + FRotator(-13.0f, 0.0f, 0.0f); // Pitch만 조정
	NewRotation.Yaw += 180.0f;

	CameraActor->SetActorLocation(NewLocation);
	CameraActor->SetActorRotation(NewRotation);
	

	SetViewTargetWithBlend(CameraActor, 1.0f); // 부드러운 전환

	UE_LOG(TestGame, Display, TEXT("Camera moved to world position of selected actor."));
}

void ACharacterSelectController::MoveCameraToDefault()
{
	// 해당 레벨에서 SelectCameraActor 찾기
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
	// 목표 위치/회전
	// (X=-800.000000,Y=-60.000000,Z=180.000000)   (Pitch=6.000000,Yaw=0.000000,Roll=0.000000)
	FVector DefaultLocation = FVector(-879.0f, -60.0f, 180.0f);
	FRotator DefaultRotation = FRotator(6.0f, 0.0f, 0.0f);

	CameraActor->SetActorLocation(DefaultLocation);
	CameraActor->SetActorRotation(DefaultRotation);

	SetViewTargetWithBlend(CameraActor, 1.0f); // 부드러운 전환
}
