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

	//에디터 모드에서 방향을 볼 수 있도록 
	// 화살하나를 생성하고 모양과 충돌등을 정의한다

	//스플라인컴포넌트를 생성한다 (패트롤을 담당한다
	mPatrolPath = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolPath"));
	mPatrolPath->SetupAttachment(mRoot);


#if WITH_EDITORONLY_DATA

	mArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DirectionArrow"));

	mArrow->ArrowColor = FColor(255, 0, 255);	//색상을 지정한다
	mArrow->bTreatAsASprite = true;				//화살표를 2D 스프라이트로 보이게 한다
	mArrow->bIsScreenSizeScaled = true;			//화살표의 크기가 화면 크기에 따라 조정된다.
	mArrow->SetSimulatePhysics(false);			//물리연산 끔

	mArrow->SetupAttachment(mRoot);				//mRoot 로 부착한다
#endif

	mRoot->bVisualizeComponent = true;

	//스폰 시 재생할 파티클을 로드한다
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		SpawnParticleAsset(TEXT(
			"/Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Shared/P_MinionSpawn.P_MinionSpawn'"));
	//성공하면 들고 있게 한다
	if (SpawnParticleAsset.Succeeded())
		mSpawnParticle = SpawnParticleAsset.Object;
}

// Called when the game starts or when spawned
void AMonsterSpawnPoint::BeginPlay()
{
	Super::BeginPlay();

	//스플라인 컴포넌트에서 스플라인을 구성하고 있는 각 포인트를 얻어온다.
	int32 Count = mPatrolPath->GetNumberOfSplinePoints();

	for (int32 i = 0; i < Count; i++)
	{
		//스플라인 포인트로 부터 월드 위치를 가져온다
		FVector Point = mPatrolPath->GetLocationAtSplinePoint(i, 
			ESplineCoordinateSpace::World);

		//얻어온 point 를 TArray 에 추가한다
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
	//타이머를 제거한다
	GetWorldTimerManager().ClearTimer(mSpawnTimer);

	//몬스터를 생성한다
	SpawnMonster();
}

void AMonsterSpawnPoint::SpawnMonster()
{
	//if (!IsValid(GetWorld()))
	//{
	//	// 안전하게 리턴 또는 대체 처리
	//	return;
	//}
	//편집 상태에서 엑터가 어느 방향을 보고 있는지 알 수 있게끔 처리한다
	if (IsValid(mSpawnMonster))
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			mSpawnParticle,
			GetActorLocation());
	}

	FActorSpawnParameters	param;
	//충돌이 된다면 그렇지 않은 위치로 스폰하게 끔 한다
	param.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;	

	//루트컴포넌트의 지점이 중간정도에 있기 때문에 처리해야 한다.
	//캡슐의 절반 높이만큼 조정해준다
	FVector	SpawnLocation = GetActorLocation();

	//1. CDO 를 들고 온다
	AMonster* CDO = mSpawnClass->GetDefaultObject<AMonster>();

	//2. 캡슐 높이를 가져오고 그 절반값을 가져온다
	if (IsValid(CDO))
	{
		UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(CDO->GetRootComponent());

		SpawnLocation.Z += Capsule->GetScaledCapsuleHalfHeight();
	}

	//계산한 값을 기준으로 몬스터를 스폰한다
	mSpawnMonster = GetWorld()->SpawnActor<AMonster>(
		mSpawnClass, 
		SpawnLocation, 
		GetActorRotation(), 
		param);

	//스폰된 몬스터에 스폰포인트 지점
	mSpawnMonster->SetSpawnPoint(this);

	//1 은 사실상 이동이 없기 때문에 예외 처리하고
	//초과한 값부터 패트롤 포인트를 저장해준다
	if(mPatrolPoint.Num() > 1)
	{
		int32	Count = mPatrolPoint.Num();

		for (int32 i = 0; i < Count; i++)
		{
			mSpawnMonster->AddPatrolPoint(mPatrolPoint[i]);
		}

	}
}



