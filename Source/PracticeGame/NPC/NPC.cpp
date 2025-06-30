// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include "../Interface/CharacterPublicInterface.h"

// Sets default values
ANPC::ANPC()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	SetRootComponent(mBody);
	//캡슐 충돌 해제
	
	//Actor 전체 내비게이션에 영향주지 않게 설정
	SetCanAffectNavigationGeneration(false);

	//PrimitiveComponent 단위
	mBody->SetCanEverAffectNavigation(false);
	
	//캡슐에 부착
	mMesh->SetupAttachment(mBody);

	//팀ID 생성
	SetGenericTeamId(FGenericTeamId(TeamNPC));

	//스텐실 설정
	mMesh->SetRenderCustomDepth(true);
	//mMesh->SetCustomDepthStencilValue(1);

	//위젯 클래스 생성자 로드하기
	//mWidgetComponent->SetWidgetClass()
	static ConstructorHelpers::FClassFinder<UUserWidget> HPPlateWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/NPC/UI_HPPlate.UI_HPPlate_C'"));
	if (HPPlateWidget.Succeeded())
		mHPPlateWidgetClass = HPPlateWidget.Class;
	
	//HP 바 = 위젯 컴포넌트 방식 (3D 스타일)
	//위젯 컴포넌트 생성
	mWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidget"));
	
	//메쉬에 부착 -> 프레임 업데이트 마다 특정 본위치로 위치와 회전을 재설정해야 한다
	mWidgetComponent->SetupAttachment(mMesh);

	//위젯이 보여지는 공간을 월드로 설정
	mWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	mWidgetComponent->SetDrawSize(FVector2D(100.0f, 40.0f));

	//로드한 위젯 클래스를 컴포넌트의 위젯 클래스로 설정한다
	if(mHPPlateWidgetClass)
	{
		mWidgetComponent->SetWidgetClass(mHPPlateWidgetClass);
		mWidgetComponent->SetUsingAbsoluteRotation(true);
	}
	//위젯 컴포넌트를 메시의 HealthBar 소켓에 부착하여 따라다니게 한다
	mWidgetComponent->AttachToComponent(mMesh, FAttachmentTransformRules::KeepRelativeTransform, TEXT("HealthBar"));
	mWidgetComponent->SetRelativeLocation(FVector(0, 0, 0));

	//mWidgetComponent->SetTranslucentSortPriority(100);
	//mWidgetComponent->SetCustomDepthStencilValue(4);
	mWidgetComponent->bReceivesDecals = false;
	mWidgetComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	
}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();
	
	//3D 스타일 HP Plate 사용 시 인스턴스 얻기
	if (mWidgetComponent && mHPPlateWidgetClass)
	{
		//현재 위젯 얻기
		UUserWidget* RawWidget = mWidgetComponent->GetWidget();
		UHPPlateWidget* ThisWidget = Cast<UHPPlateWidget>(RawWidget);

		if (ThisWidget)
		{
			//개별 NPC 마다 값 초기화 하기 (이름과 HP 값)
			//NPC 에는 아직 Name/HP 와같은 데이터를 설정하지 않았다. 
			//지금은 Monster 로 국한해서 처리하도록 한다 
			if (ICharacterPublicInterface* MonsterInfo = Cast<ICharacterPublicInterface>(this))
			{
				ThisWidget->SetNPCName(MonsterInfo->GetName());
				ThisWidget->SetHPBar(MonsterInfo->GetHP(), MonsterInfo->GetMaxHP());
			}
		}
	}
	/*
	//UMG 스타일 HP Plate 위젯 표시방법
	if (mHPPlateWidgetClass)
	{
		//위젯을 뷰포트에 생성하고 뿌린다
		mHPPlateWidget = CreateWidget<UHPPlateWidget>(GetWorld(), mHPPlateWidgetClass);
		if (mHPPlateWidget)
		{
			UE_LOG(TestGame, Warning, TEXT("HPPlate : AddToViewport"));
			mHPPlateWidget->SetVisibility(ESlateVisibility::Visible);
			mHPPlateWidget->AddToViewport();
			//mHPPlateWidget->AddToViewport();
		}
	}
	*/

	//델리게이트 함수 바인딩
}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//3D 공간형 위젯 회전 수정 코드
	// 1. 카메라 Forward 벡터 사용
	FVector CameraForward = UGameplayStatics::GetPlayerController(GetWorld(), 0)
		->PlayerCameraManager->GetCameraRotation().Vector();

	// 2. 위젯이 놓일 위치 기준으로 역방향 계산
	FVector LookDirection = -CameraForward;

	// 3. 회전 행렬 구성: Forward = LookDirection, Up = 월드 Z축
	FRotator WidgetRotation = FRotationMatrix::MakeFromXZ(LookDirection, FVector::UpVector).Rotator();

	// 4. 적용
	mWidgetComponent->SetWorldRotation(WidgetRotation);
	
	/*
	//UMG 스타일 위치 설정 (머리 위에 띄우기)
	if(!mHPPlateWidget)
		return;

	if (mHPPlateWidget)
	{
		//"head", "spine_03" 등
		FVector BoneLocation = mMesh->GetBoneLocation(TEXT("HealthBar")); 
		FVector2D ScreenPos;
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (PC && PC->ProjectWorldLocationToScreen(BoneLocation, ScreenPos))
		{
			int32 ViewportX, ViewportY;
			PC->GetViewportSize(ViewportX, ViewportY);

			bool bOnScreen = ScreenPos.X >= 0 && ScreenPos.X <= ViewportX &&
				ScreenPos.Y >= 0 && ScreenPos.Y <= ViewportY;

			mHPPlateWidget->SetVisibility(bOnScreen ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			if (bOnScreen)
			{
				mHPPlateWidget->SetPositionInViewport(ScreenPos, true);
			}
		}
	}
	*/
}

void ANPC::SetGenericTeamId(
	const FGenericTeamId& TeamID)
{
	mTeamID = TeamID;
}

FGenericTeamId ANPC::GetGenericTeamId() const
{
	return mTeamID;
}

ETeamAttitude::Type ANPC::GetTeamAttitudeTowards(const AActor& Other) const
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

void ANPC::EnableOutLine(bool Enable)
{
	int32 Stencil = mMesh->CustomDepthStencilValue;

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
	mMesh->SetCustomDepthStencilValue(Stencil);
}

void ANPC::EnableOcclusion(bool Enable)
{
	int32 Stencil = mMesh->CustomDepthStencilValue;

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
	mMesh->SetCustomDepthStencilValue(Stencil);
}