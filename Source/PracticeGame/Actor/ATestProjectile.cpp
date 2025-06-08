// Fill out your copyright notice in the Description page of Project Settings.


#include "ATestProjectile.h"



// Sets default values
AATestProjectile::AATestProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//프로젝타일 무브먼트 서브 컴포넌트 생성
	mProjectMovement = CreateDefaultSubobject
		<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	//스태틱 메쉬 서브 컴포넌트 생성
	mStaticMesh = CreateDefaultSubobject
		<UStaticMeshComponent>(TEXT("Static Mesh"));

	//스태틱 메쉬 하나 로드(화살표모양)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		StaticMesh(TEXT(
		"/Script/Engine.StaticMesh'/Engine/EditorResources/FieldNodes/_Resources/SM_FieldArrow.SM_FieldArrow'")
	);

	//로드 성공하면 스태틱메쉬를 컴포넌트에 설정
	if (StaticMesh.Succeeded())
		mStaticMesh->SetStaticMesh(StaticMesh.Object);

	//스태틱 메쉬를 루트컴포넌트로 설정
	SetRootComponent(mStaticMesh);

	//투사체 속도 설정
	mProjectMovement->InitialSpeed = 600.f;
	mProjectMovement->MaxSpeed = 3000.f;
	
	//중력 영향 제거
	mProjectMovement->ProjectileGravityScale = .0f;
	
	//투사체 튕기지 않게 설정
	mProjectMovement->bShouldBounce = false;

	////충돌 컴포넌트 생성
	//mCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));

	//
	////트리거박스를  루트에 부착한다
	//mCollisionBox->SetupAttachment(GetRootComponent());

	///* 콜리전 박스의 트랜스폼 설정
	//	콜리전 박스 크기
	//		(X=38.475686,Y=8.939726,Z=8.999064)
	//			상대위치
	//			(X = 39.000000, Y = 0.000000, Z = 0.000000)
	//*/
	//mCollisionBox->SetBoxExtent(FVector(38.475686f, 8.939726f, 8.999064f));
	//mCollisionBox->SetRelativeLocation(FVector(39, 0, 0));

	//

	////충돌 프로파일 지정
	//mCollisionBox->SetCollisionProfileName(TEXT("PhysicsActor"));
	//
	////충돌 시뮬레이션 이벤트 생성 체크
	//mCollisionBox->SetSimulatePhysics(false);
	//mCollisionBox->SetCollisionResponseToChannels(ECC_PhysicsBody, ECR_Block);
	
	//스태틱 메시 컴포넌트에서 충돌 처리
	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mStaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
	mStaticMesh->SetSimulatePhysics(false);
	mStaticMesh->SetNotifyRigidBodyCollision(true);
	//오버렙이벤트 생성 On
	mStaticMesh->SetGenerateOverlapEvents(true);
	//PhysicsBody 에 대한 응답 설정
	mStaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	//아래를 설정하면 OnComponentHit 호출가능
	mProjectMovement->bSweepCollision = true;

}

// Called when the game starts or when spawned
void AATestProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	OnActorHit .AddDynamic(
			this, &AATestProjectile::OnHit);
}

// Called every frame
void AATestProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AATestProjectile::OnHit(
	AActor* SelfActor, 
	AActor* OtherActor,
	FVector NormalImpulse, 
	const FHitResult& Hit)
{
	
	//충돌 시 투사체를 제거한다
	if (OtherActor && OtherActor != this)
	{
		Destroy();
		UE_LOG(TestGame, Display, TEXT("****A Projectile Collided and Destroyed"));
	}
}

