// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueProjectileA.h"
#include "../NPC/Monster/Monster.h"
#include "../Player/PlayerCharacter.h"
#include "../Interface/CharacterRenderingInterface.h"


// Sets default values
ARogueProjectileA::ARogueProjectileA()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//충돌 컴포넌트 생성 및 설정
	mCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	mCollisionComponent->InitSphereRadius(30.f);
	mCollisionComponent->SetCollisionProfileName(TEXT("PlayerProjectile"));
	mCollisionComponent->OnComponentHit.AddDynamic(this, &ARogueProjectileA::OnHit);
	RootComponent = mCollisionComponent;

	//프로젝타일 무브먼트 서브 컴포넌트 생성
	mProjectMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	mProjectMovement->ProjectileGravityScale = 1.0f; // 중력 작용
	mProjectMovement->InitialSpeed = 1000.f;
	mProjectMovement->MaxSpeed = 3000.f;
	mProjectMovement->bRotationFollowsVelocity = true;
	
	//나이아가라컴포넌트 생성 (투사체 형태)
	mNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	mNiagaraComponent->SetupAttachment(RootComponent);

	//나이아가라 시스템 로드 및 주소 할당
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/MixedVFX/Particles/Projectiles/NS_GalaxyArrow.NS_GalaxyArrow'"));
	if (NiagaraAsset.Succeeded())
		mNiagaraSystem = NiagaraAsset.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ImpactAsset1(TEXT("/Script/Niagara.NiagaraSystem'/Game/MixedVFX/Particles/Projectiles/Hits/NS_GalaxyArrow_Hit.NS_GalaxyArrow_Hit'"));
	if (ImpactAsset1.Succeeded())
		mImpactVFX = ImpactAsset1.Object;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ArrowDisappearVFX(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonSparrow/FX/Particles/Sparrow/Skins/Rogue/P_Sparrow_Primary_Ballistic_HitWorld_WaterImpact.P_Sparrow_Primary_Ballistic_HitWorld_WaterImpact'"));
	if (ArrowDisappearVFX.Succeeded())
		mArrowDisappearVFX = ArrowDisappearVFX.Object;
}

// Called when the game starts or when spawned
void ARogueProjectileA::BeginPlay()
{
	Super::BeginPlay();
	
	if (mNiagaraSystem)
	{
		mNiagaraComponent->SetAsset(mNiagaraSystem);
		mNiagaraComponent->ActivateSystem(true);
	}
}

// Called every frame
void ARogueProjectileA::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARogueProjectileA::InitProjectile(FVector Direction, float Speed)
{
	if (mProjectMovement)
	{
		mProjectMovement->Velocity = Direction * Speed;
	}
}

void ARogueProjectileA::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//기본 충돌 이펙트
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		mImpactVFX,
		Hit.ImpactPoint,
		Hit.ImpactNormal.Rotation()
	);
	
	//화살 서서히 사라지는 이펙트
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
		mArrowDisappearVFX, Hit.ImpactPoint,
		Hit.ImpactNormal.Rotation());

	//사운드 재생
	USoundBase* SFXAsset = LoadObject<USoundBase>(
		GetWorld(),
		TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/arrow-impact-87260.arrow-impact-87260'"));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SFXAsset,
		Hit.ImpactPoint, FRotator::ZeroRotator,
		1.f);

	AMonster* HitTarget = Cast<AMonster>(Hit.GetActor());

	//몬스터 타깃 적중 시 
	if (HitTarget)
	{

		FDamageEvent DamageEvent;
		//  데미지 처리 Hit.GetActor()->TakeDamage(5, DamageEvent, )
		HitTarget->TakeDamage(25.0f, DamageEvent, GetInstigatorController(), this);
	}
	
	Destroy();
}

