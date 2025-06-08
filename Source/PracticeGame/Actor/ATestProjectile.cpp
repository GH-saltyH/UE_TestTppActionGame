// Fill out your copyright notice in the Description page of Project Settings.


#include "ATestProjectile.h"



// Sets default values
AATestProjectile::AATestProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//������Ÿ�� �����Ʈ ���� ������Ʈ ����
	mProjectMovement = CreateDefaultSubobject
		<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	//����ƽ �޽� ���� ������Ʈ ����
	mStaticMesh = CreateDefaultSubobject
		<UStaticMeshComponent>(TEXT("Static Mesh"));

	//����ƽ �޽� �ϳ� �ε�(ȭ��ǥ���)
	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		StaticMesh(TEXT(
		"/Script/Engine.StaticMesh'/Engine/EditorResources/FieldNodes/_Resources/SM_FieldArrow.SM_FieldArrow'")
	);

	//�ε� �����ϸ� ����ƽ�޽��� ������Ʈ�� ����
	if (StaticMesh.Succeeded())
		mStaticMesh->SetStaticMesh(StaticMesh.Object);

	//����ƽ �޽��� ��Ʈ������Ʈ�� ����
	SetRootComponent(mStaticMesh);

	//����ü �ӵ� ����
	mProjectMovement->InitialSpeed = 600.f;
	mProjectMovement->MaxSpeed = 3000.f;
	
	//�߷� ���� ����
	mProjectMovement->ProjectileGravityScale = .0f;
	
	//����ü ƨ���� �ʰ� ����
	mProjectMovement->bShouldBounce = false;

	////�浹 ������Ʈ ����
	//mCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));

	//
	////Ʈ���Źڽ���  ��Ʈ�� �����Ѵ�
	//mCollisionBox->SetupAttachment(GetRootComponent());

	///* �ݸ��� �ڽ��� Ʈ������ ����
	//	�ݸ��� �ڽ� ũ��
	//		(X=38.475686,Y=8.939726,Z=8.999064)
	//			�����ġ
	//			(X = 39.000000, Y = 0.000000, Z = 0.000000)
	//*/
	//mCollisionBox->SetBoxExtent(FVector(38.475686f, 8.939726f, 8.999064f));
	//mCollisionBox->SetRelativeLocation(FVector(39, 0, 0));

	//

	////�浹 �������� ����
	//mCollisionBox->SetCollisionProfileName(TEXT("PhysicsActor"));
	//
	////�浹 �ùķ��̼� �̺�Ʈ ���� üũ
	//mCollisionBox->SetSimulatePhysics(false);
	//mCollisionBox->SetCollisionResponseToChannels(ECC_PhysicsBody, ECR_Block);
	
	//����ƽ �޽� ������Ʈ���� �浹 ó��
	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	mStaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
	mStaticMesh->SetSimulatePhysics(false);
	mStaticMesh->SetNotifyRigidBodyCollision(true);
	//�������̺�Ʈ ���� On
	mStaticMesh->SetGenerateOverlapEvents(true);
	//PhysicsBody �� ���� ���� ����
	mStaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Block);
	//�Ʒ��� �����ϸ� OnComponentHit ȣ�Ⱑ��
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
	
	//�浹 �� ����ü�� �����Ѵ�
	if (OtherActor && OtherActor != this)
	{
		Destroy();
		UE_LOG(TestGame, Display, TEXT("****A Projectile Collided and Destroyed"));
	}
}

