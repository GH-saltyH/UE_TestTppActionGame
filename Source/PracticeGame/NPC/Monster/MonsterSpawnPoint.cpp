// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSpawnPoint.h"
//#include "Particles/ParticleSystemComponent.h"
#include "Monster.h"


// Sets default values
AMonsterSpawnPoint::AMonsterSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SetRootComponent(mRoot);

	//������ ��忡�� ������ �� �� �ֵ��� 
	// ȭ���ϳ��� �����ϰ� ���� �浹���� �����Ѵ�

	//���ö���������Ʈ�� �����Ѵ� (��Ʈ���� ����Ѵ�
	mPatrolPath = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolPath"));
	mPatrolPath->SetupAttachment(mRoot);


#if WITH_EDITORONLY_DATA

	mArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));

	mArrow->ArrowColor = FColor(255, 0, 255);	//������ �����Ѵ�
	mArrow->bTreatAsASprite = true;				//ȭ��ǥ�� 2D ��������Ʈ�� ���̰� �Ѵ�
	mArrow->bIsScreenSizeScaled = true;			//ȭ��ǥ�� ũ�Ⱑ ȭ�� ũ�⿡ ���� �����ȴ�.
	mArrow->SetSimulatePhysics(false);			//�������� ��

	mArrow->SetupAttachment(mRoot);				//mRoot �� �����Ѵ�
#endif

	mRoot->bVisualizeComponent = true;

	//���� �� ����� ��ƼŬ�� �ε��Ѵ�
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		SpawnParticleAsset(TEXT(
			"/Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_MinionSpawn.P_MinionSpawn'"));
	//�����ϸ� ��� �ְ� �Ѵ�
	if (SpawnParticleAsset.Succeeded())
		mSpawnParticle = SpawnParticleAsset.Object;
}

// Called when the game starts or when spawned
void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	//���ö��� ������Ʈ���� ���ö����� �����ϰ� �ִ� �� ����Ʈ�� ���´�.
	int32 Count = mPatrolPath->GetNumberOfSplinePoints();

	for (int32 i = 0; i < Count; i++)
	{
		//���ö��� ����Ʈ�� ���� ���� ��ġ�� �����´�
		FVector Point = mPatrolPath->GetLocationAtSplinePoint(i, 
			ESplineCoordinateSpace::World);

		//���� point �� TArray �� �߰��Ѵ�
		mPatrolPoint.Add(Point);
	}
	SpawnMonster();
}

// Called every frame
void AMonsterSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterSpawnPoint::SpawnTimer()
{
	//Ÿ�̸Ӹ� �����Ѵ�
	GetWorldTimerManager().ClearTimer(mSpawnTimer);

	//���͸� �����Ѵ�
	SpawnMonster();
}

void AMonsterSpawnPoint::SpawnMonster()
{
	//if (!IsValid(GetWorld()))
	//{
	//	// �����ϰ� ���� �Ǵ� ��ü ó��
	//	return;
	//}
	//���� ���¿��� ���Ͱ� ��� ������ ���� �ִ��� �� �� �ְԲ� ó���Ѵ�
	if (IsValid(mSpawnMonster))
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			mSpawnParticle,
			GetActorLocation());
	}

	FActorSpawnParameters	param;
	//�浹�� �ȴٸ� �׷��� ���� ��ġ�� �����ϰ� �� �Ѵ�
	param.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;	

	//��Ʈ������Ʈ�� ������ �߰������� �ֱ� ������ ó���ؾ� �Ѵ�.
	//ĸ���� ���� ���̸�ŭ �������ش�
	FVector	SpawnLocation = GetActorLocation();

	//1. CDO �� ��� �´�
	AMonster* CDO = mSpawnClass->GetDefaultObject<AMonster>();

	//2. ĸ�� ���̸� �������� �� ���ݰ��� �����´�
	if (IsValid(CDO))
	{
		UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(CDO->GetRootComponent());

		SpawnLocation.Z += Capsule->GetScaledCapsuleHalfHeight();
	}

	//����� ���� �������� ���͸� �����Ѵ�
	mSpawnMonster = GetWorld()->SpawnActor<AMonster>(
		mSpawnClass, 
		SpawnLocation, 
		GetActorRotation(), 
		param);

	//������ ���Ϳ� ��������Ʈ ����
	mSpawnMonster->SetSpawnPoint(this);

	//1 �� ��ǻ� �̵��� ���� ������ ���� ó���ϰ�
	//�ʰ��� ������ ��Ʈ�� ����Ʈ�� �������ش�
	if(mPatrolPoint.Num() > 1)
	{
		int32	Count = mPatrolPoint.Num();

		for (int32 i = 0; i < Count; i++)
		{
			mSpawnMonster->AddPatrolPoint(mPatrolPoint[i]);
		}

	}
}



