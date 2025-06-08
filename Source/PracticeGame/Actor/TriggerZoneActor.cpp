// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerZoneActor.h"


// Sets default values
ATriggerZoneActor::ATriggerZoneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//����ƽ �޽� ������Ʈ�� ���������Ʈ�� �����ϰ� �޾Ƶд�
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>
		(TEXT("StaticMesh"));

	//����ƽ �޽��� ���� Shape ������ �ε��Ѵ�
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		StaticMeshAsset(
			TEXT(
			"/Script/Engine.StaticMesh'/Game/Shape/Cube.Cube'")
		);

	//�ε忡 �����ϸ� �޽� ������Ʈ�� �����Ѵ�
	if (StaticMeshAsset.Succeeded())
		mStaticMesh->SetStaticMesh(StaticMeshAsset.Object);

	//�ڽ� ������Ʈ�� ���������Ʈ�� �����ϰ� �޾Ƶд�
	mTriggerBox = CreateDefaultSubobject<UBoxComponent>
		(TEXT("TriggerBox"));

	//�޽��� ��Ʈ ������Ʈ�� �����Ѵ�
	SetRootComponent(mStaticMesh);

	//Ʈ���Źڽ���  ��Ʈ�� �����Ѵ�
	mTriggerBox->SetupAttachment(GetRootComponent());

	/* �޽� �� Ʈ���� �ڽ��� Ʈ������ ���� 
		�ڽ� ������ 
			(X=1.250000,Y=1.250000,Z=0.050000)
		Ʈ���� �ڽ� ũ��
			(X=50.000000,Y=50.000000,Z=32.000000)
				�����ġ
				(X=0.000000,Y=0.000000,Z=83.000000)
	*/
	mStaticMesh->SetRelativeScale3D(FVector(1.25f, 1.25f, 0.05f));
	mTriggerBox->SetBoxExtent(FVector(50, 50, 32));
	mTriggerBox->SetRelativeLocation(FVector(0, 0, 83));

	//�浹 �������� ����
	mTriggerBox->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ATriggerZoneActor::BeginPlay()
{
	Super::BeginPlay();
	
	//�� ��������Ʈ �Լ��� 
	// mTriggerBox �� Overlap �̺�Ʈ���� 
	// TriggerBoxOnOverlap �� ȣ���ϰ� �ȴ�
	mTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerZoneActor::TriggerBoxOnOverlap);
}

//mTriggerBox �� Overlap �� �� ȣ��Ǵ� �Լ�
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

