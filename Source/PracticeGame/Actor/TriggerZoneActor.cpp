// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerZoneActor.h"


// Sets default values
ATriggerZoneActor::ATriggerZoneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//스테틱 메쉬 컴포넌트를 서브오브젝트로 생성하고 받아둔다
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>
		(TEXT("StaticMesh"));

	//스태틱 메쉬를 위한 Shape 에셋을 로드한다
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		StaticMeshAsset(
			TEXT(
			"/Script/Engine.StaticMesh'/Game/Shape/Cube.Cube'")
		);

	//로드에 성공하면 메쉬 컴포넌트에 적용한다
	if (StaticMeshAsset.Succeeded())
		mStaticMesh->SetStaticMesh(StaticMeshAsset.Object);

	//박스 컴포넌트를 서브오브젝트로 생성하고 받아둔다
	mTriggerBox = CreateDefaultSubobject<UBoxComponent>
		(TEXT("TriggerBox"));

	//메쉬를 루트 컴포넌트로 지정한다
	SetRootComponent(mStaticMesh);

	//트리거박스를  루트에 부착한다
	mTriggerBox->SetupAttachment(GetRootComponent());

	/* 메쉬 및 트리거 박스의 트랜스폼 설정 
		박스 스케일 
			(X=1.250000,Y=1.250000,Z=0.050000)
		트리거 박스 크기
			(X=50.000000,Y=50.000000,Z=32.000000)
				상대위치
				(X=0.000000,Y=0.000000,Z=83.000000)
	*/
	mStaticMesh->SetRelativeScale3D(FVector(1.25f, 1.25f, 0.05f));
	mTriggerBox->SetBoxExtent(FVector(50, 50, 32));
	mTriggerBox->SetRelativeLocation(FVector(0, 0, 83));

	//충돌 프로파일 지정
	mTriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ATriggerZoneActor::BeginPlay()
{
	Super::BeginPlay();
	
	//이 델리게이트 함수는 
	// mTriggerBox 의 Overlap 이벤트에서 
	// TriggerBoxOnOverlap 을 호출하게 된다
	mTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerZoneActor::TriggerBoxOnOverlap);
}

//mTriggerBox 의 Overlap 일 때 호출되는 함수
void ATriggerZoneActor::TriggerBoxOnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsTriggered)
	{
		if (GEngine)
		{
			GEngine->
				AddOnScreenDebugMessage(
					-1,
					1.f,
					FColor::Red,
					TEXT("TriggerBoxOnOverlap has been called : Already Triggered")
				);
			return;
		}
	}
	if (GEngine)
	{
		GEngine->
			AddOnScreenDebugMessage(
				-1,
				1.f,
				FColor::Green,
				TEXT("TriggerBoxOnOverlap has been called : Just Triggered")
			);
		bIsTriggered = true;
	}
}

// Called every frame
void ATriggerZoneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

