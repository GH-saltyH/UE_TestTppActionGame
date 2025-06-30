// Fill out your copyright notice in the Description page of Project Settings.


#include "Ranger.h"
#include "../../Actor/RogueProjectileA.h"
#include "../../NPC/Monster/Monster.h"

ARanger::ARanger()
{
	mDataKey = TEXT("Ranger");
	/*
	* 메시 및 캡슐 설정
	*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMesh(TEXT(
			"/Script/Engine.SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Skins/Rogue/Meshes/Sparrow_Rogue.Sparrow_Rogue'"));
	if (SkeletalMesh.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(SkeletalMesh.Object);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.5f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.5f);

	//무기 콜리전 박스 생성
	//도적은 쌍수무기를 들고 있다. 두개의 콜리전 박스를 각각 생성한다
	for (int32 i = 0; i < 2; i++)
	{
		UBoxComponent* NewBox;
		if (i == 0)
		{
			NewBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponColBox_R"));
			NewBox->SetupAttachment(GetMesh(), TEXT("weapon_r_collider"));
		}
		else
		{
			NewBox = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponColBox_L"));
			NewBox->SetupAttachment(GetMesh(), TEXT("weapon_l_collider"));
		}

		NewBox->SetBoxExtent(FVector(0.1f, 32.5839825f, 1.594249f));
		NewBox->SetHiddenInGame(false);

		NewBox->SetGenerateOverlapEvents(true);
		NewBox->SetCollisionProfileName(TEXT("PlayerMeleeWeaponOverlap"));
		//NewBox->SetSimulatePhysics(true);
		//NewBox->SetNotifyRigidBodyCollision(true);
		//NewBox->SetCollisionProfileName(TEXT("PlayerMeleeWeaponBlock"));
		NewBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		NewBox->bHiddenInGame = false;
		//NewBox->SetWalkableSlopeOverride(EWalkableSlopeBehavior::WalkableSlope_Unwalkable);

		//멤버에 객체를 할당한다
		mWeaponColBox.Add(NewBox);
	}

	//JumpZVelocity 점프 높이를 설정한다
	GetCharacterMovement()
		->JumpZVelocity = 600.f;
	//GravityScale  중력을 설정한다
	GetCharacterMovement()
		->GravityScale = 1.0f;
	//AirControl  공중 이동 제어를 설정한다, 
	// 낮을 수록 이동이 제한되고, 높을수록 공중 이동에 자유로워진다 
	GetCharacterMovement()
		->AirControl = 0.5f;

	//JumpMaxCount 는  착지하기 전까지 최대 가능한 점프 횟수를 결정한다
	JumpMaxCount = 2;

	//	애니메이션 블루프린트 에셋 로드
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		ABPAsset(TEXT(
			"/Script/Engine.AnimBlueprint'/Game/Player/Ranger/ABPRanger_T.ABPRanger_T_C'"));
	if (ABPAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(ABPAsset.Class);

	//투사체 에셋로드
	static ConstructorHelpers::FClassFinder<AATestProjectile> TestProjectile(TEXT(
		"/Script/CoreUObject.Class'/Script/PracticeGame.ATestProjectile'"));
	if (TestProjectile.Succeeded())
		mProjectileClass = TestProjectile.Class;

	//뱀파이어 슬래시 (스킬로 쓸 것
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AssetVampireSlash(
		TEXT("/Script/Niagara.NiagaraSystem'/Game/MixedVFX/Particles/Slashes/NS_VampireSlash.NS_VampireSlash'"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> CleaveVFXAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/MixedVFX/Particles/Slashes/SeparateParts/Hits/NS_VampireSlash_Hit.NS_VampireSlash_Hit'"));
	if (CleaveVFXAsset.Succeeded())
		mNiagaraCleaveHit = CleaveVFXAsset.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Skill1ProjectileAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/MixedVFX/Particles/Projectiles/NS_Projectile_04.NS_Projectile_04'"));
	if (Skill1ProjectileAsset.Succeeded())
		mSkill1ProjectileAsset = Skill1ProjectileAsset.Object;

	//나이아가라인지 파티클인지 선택
	mTrailAssetMode = ETrailAssetMode::NIAGARASYSTEM;

	if (mTrailAssetMode == ETrailAssetMode::NIAGARASYSTEM)
	{
		//트레일 파티클 로드 (왼손 오른손 각각)
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> LeftTrailAssetNiagara(
			TEXT("/Script/Niagara.NiagaraSystem'/Game/Player/Animation/FX/NS_weaponTrail.NS_weaponTrail'"));

		//각각 다른 에셋을 써야 하는 경우
		// static ConstructorHelpers::FObjectFinder<UNiagaraComponent> RightTrailAssetNiagara(
		// TEXT(""));

		if (LeftTrailAssetNiagara.Succeeded())
		{
			mTrailEffectNiagaraL = LeftTrailAssetNiagara.Object;
			mTrailEffectNiagaraR = LeftTrailAssetNiagara.Object;
		}
		//각각 다른 에셋을 써야 하는 경우
		/*if (RightTrailAssetNiagara.Succeeded())
		{
			mTrailEffectR = RightTrailAssetNiagara.Object;
		}*/
	}
	else if (mTrailAssetMode == ETrailAssetMode::PARTICLESYSTEM)
	{
		//트레일 파티클 로드 (왼손 오른손 각각)
		static ConstructorHelpers::FObjectFinder<UParticleSystem> LeftTrailAsset(
			TEXT("/Script/Engine.MaterialInstanceConstant'/Game/MixedVFX/Materials/MaterialInstances/MI_Slash_01.MI_Slash_01'"));
		//각각 다른 에셋을 써야 하는 경우
		// static ConstructorHelpers::FObjectFinder<UParticleSystem> RightTrailAsset(
		// TEXT(""));

		if (LeftTrailAsset.Succeeded())
		{
			mTrailEffectL = LeftTrailAsset.Object;
			mTrailEffectR = LeftTrailAsset.Object;
		}
		//각각 다른 에셋을 써야 하는 경우
		/*if (RightTrailAsset.Succeeded())
		{
			mTrailEffectR = RightTrailAsset.Object;
		}*/
	}

	//생성자에서 로드할 데칼들
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DecalAsset(TEXT("/Script/Engine.Material'/Game/Materials/Rogue/MT_RogueAoEDecal.MT_RogueAoEDecal'"));
	if (DecalAsset.Succeeded())
		mSkill1DecalAsset = DecalAsset.Object;
}

void ARanger::NormalAttack()
{
	//오버렙 방식일 때 
	if (bNormalAttackOverlap)
	{

	}
	else
	{
		UParticleSystem* VFXAsset = LoadObject<UParticleSystem>(
			GetWorld(),
			TEXT("/Script/Engine.ParticleSystem'/Game/Player/VFX/P_Terra_DoubleSweep_Impact.P_Terra_DoubleSweep_Impact'"));

		USoundBase* SFXAsset = LoadObject<USoundBase>(
			GetWorld(),
			TEXT("Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_swordslashandswing.sfx_swordslashandswing'"));

		float DamagePoint = 20.0f;
		float SphereSize = 150.0f;
		float SphereDistance = 0.0f;
		float AngleStartDistance = 25.0f;
		float InnerAngle = 80.0f;

		//위의 정보를 바탕으로 Cone 스윕 처리를 한다 ->  구 생성 -> 정의역구간(각도) 생성 -> 검출
		RunSweepConeAndDamage(DamagePoint, SphereSize, SphereDistance, -AngleStartDistance, InnerAngle, ECollisionChannel::ECC_GameTraceChannel7, VFXAsset, true, SFXAsset, true);
	}
}

void ARanger::SpecialAttack()
{
	//사운드생성 : 비동기방식- 런타임 로드
	USoundBase* LaunchSFX = LoadObject<USoundBase>(
		GetWorld(),
		TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_boom01.sfx_boom01'"));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), LaunchSFX,
		GetMesh()->GetComponentLocation(), FRotator::ZeroRotator,
		2.f);

	float SphereSize = 400.f;			//생성할 구의 크기

	// 1. 케릭터의 전방 방향을 구하고 각도를 계산할 위치를 잡아준다Reference Point
	FVector SpherePoint = GetActorLocation(); // Sweep 시작 위치
	FVector SweepEnd = SpherePoint;

	// 2. 설정한 반경(SphereSize) 크기의 스피어 Sweep 을 실행한다
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereSize);
	TArray<FHitResult> HitResults;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		SpherePoint,														//구를 만들 시작 포인트
		SweepEnd,														//종료 포인트 (구체이므로 같은곳을 지정하였다
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,		// PlayerWeaponSweep -> 트레이스 채널
		SphereShape														//SphereSize 크기의 Sphere 로 만든다
	);
	if (CVarDebugLines.GetValueOnGameThread() == 1) // 값이 1이면 디버그 출력
	{

		// 스피어 디버그 드로잉 (Sweep 검사 범위)
		float DrawTime = 2.0f;
		DrawDebugSphere(
			GetWorld(),
			SpherePoint,					// 구체 중심
			SphereSize,					// 반지름
			12,								// 세그먼트 수 (디테일 정도)
			FColor::Red,					// 색상
			false,
			DrawTime 						// 지속 시간
		);
	}
	//최적화 : 충돌 없으면 나가기
	if (!bHit)
		return;

	// 3. 모든 충돌체 탐색
	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		//최적화 : 충돌 가능한 엑터가 아니면 건너뛰기 
		if (!HitActor)
			continue;

		//몬스터 및 하위 클래스 인 경우에 충돌 처리
		if (AMonster* Monster = Cast<AMonster>(HitActor))
		{
			//파티클 생성 : 비동기방식 런타임 로드  
			UParticleSystem* HitParticle = LoadObject<UParticleSystem>(
				GetWorld(),
				TEXT("/Script/Engine.ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BladeSiphon/FX/p_Countess_BladeSiphon_HitFX.p_Countess_BladeSiphon_HitFX'"));

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				HitParticle, Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation());

			UE_LOG(TestGame, Warning, TEXT("Hit Actor : [%s] "), *HitActor->GetName());
			FDamageEvent	DmgEvent;
			HitActor->TakeDamage(100.f, DmgEvent, GetController(), this);
		}
	}
}

void ARanger::CleaveAttack()
{
	GetWorld()->GetTimerManager().SetTimer(
		mCleaveReleaseTimer,
		this,
		&ARanger::ReleaseCleave,
		0.36f,
		false);
}

void ARanger::ReleaseCleave()
{

	UParticleSystem* VFXAsset = LoadObject<UParticleSystem>(
		GetWorld(),
		TEXT("/Script/Engine.ParticleSystem'/Game/ParagonCountess/FX/Particles/Abilities/BladeSiphon/FX/p_Countess_BladeSiphon_HitFX.p_Countess_BladeSiphon_HitFX'"));

	USoundBase* SFXAsset = LoadObject<USoundBase>(
		GetWorld(),
		TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_impact_armor_metal.sfx_impact_armor_metal'"));

	float DamagePoint = 30.0f;
	float SphereSize = 150.0f;
	float SphereDistance = 0.0f;
	float AngleStartDistance = 20.0f;
	float InnerAngle = 95.0f;

	//위의 정보를 바탕으로 Cone 스윕 처리를 한다 ->  구 생성 -> 정의역구간(각도) 생성 -> 검출
	RunSweepConeAndDamage(DamagePoint, SphereSize, SphereDistance, -AngleStartDistance, InnerAngle, ECollisionChannel::ECC_GameTraceChannel7, VFXAsset, true, SFXAsset, true);
}

void ARanger::MakePreviewCastZone()
{
	UE_LOG(TestGame, Warning, TEXT("Entered MakePreviewCastZone"));
	//영역표기를 위해 주기적으로 업데이트하는 함수를 호출한다.
	GetWorld()->GetTimerManager().SetTimer(
		mSkillTimer1,
		this,
		&ARanger::UpdateSkill1Selection,
		0.05f,
		true,
		0.01f);
	if (mSkillTimer1.IsValid())
		UE_LOG(TestGame, Warning, TEXT("mSkillTimer1 was set"));
}

void ARanger::CancelPreviewCastZone()
{
	DeleteSkill1Decal();
	if (mSkillTimer1.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(mSkillTimer1);

	//하이라이트된 녀석들 지워주기
	if (!mPrevDetectedActors.IsEmpty())
	{
		for (auto& Prev : mPrevDetectedActors)
		{
			Prev->EnableOcclusion(false);
			Prev->EnableOutLine(false);
		}

		mPrevDetectedActors.Empty();
	}
}

void ARanger::AcceptPreviewCastZone()
{
	Skill1Init();
	DeleteSkill1Decal();
	if (mSkillTimer1.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(mSkillTimer1);
	//하이라이트된 녀석들 지워주기
	if (!mPrevDetectedActors.IsEmpty())
	{
		for (auto& Prev : mPrevDetectedActors)
		{
			Prev->EnableOcclusion(false);
			Prev->EnableOutLine(false);
		}

		mPrevDetectedActors.Empty();
	}
}

void ARanger::UpdateSkill1Selection()
{
	float SphereRadius = 300.f;

	//플레이어 컨트롤러가 널포인터거나 지움예약/GC 인경우 리턴
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(PlayerController))
		return;

	FVector WorldLocation, WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// 레이캐스트 수행하여 실제 월드 좌표 얻기
		FHitResult HitResult;
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f); // 적절한 거리 설정

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // 캐릭터 제외

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			WorldLocation = HitResult.Location; // 실제 충돌 지점으로 업데이트

			if (CVarDebugLines.GetValueOnGameThread() == 1) // 값이 1이면 디버그 출력
			{
				// 스피어 디버그 드로잉 (Sweep 검사 범위)
				float DrawTime = 0.05f;
				DrawDebugSphere(
					GetWorld(),
					WorldLocation,					// 구체 중심
					SphereRadius,								// 반지름
					12,								// 세그먼트 수 (디테일 정도)
					FColor::Green,					// 색상
					false,
					DrawTime 						// 지속 시간
				);
			}

			//데칼을 생성한다 -> 해당 함수는 있으면 위치를 업데이트한다
			SpawnSkill1DecalAtLocation(WorldLocation, FVector(SphereRadius, SphereRadius, SphereRadius));

			////데칼을 생성한다(동기 로드)
			//UMaterialInterface* DecalAsset = LoadObject<UMaterialInterface>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/ParagonMinions/FX/Textures/Runes/T_Decal_Astro.T_Decal_Astro'"));

			//UDecalComponent* Decal = UGameplayStatics::SpawnDecalAtLocation(
			//	GetWorld(),
			//	DecalAsset,
			//	FVector(SphereRadius, SphereRadius, SphereRadius),
			//	WorldLocation,
			//	FRotator::ZeroRotator
			//	));

			//	if (Decal)
			//	{
			//		Decal->SetFadeScreenSize(0.1f);  // 일정 거리에서 페이드 아웃
			//		Decal->SetWorldLocation(WorldLocation);
			//	}

			//UE_LOG(TestGame, Warning, TEXT("UpdateSkill1Selection : succeeded collision check"));

			// 이전에 부딪혔던 목록은 모두 비활성화한다.
			if (!mPrevDetectedActors.IsEmpty())
			{
				for (auto& Prev : mPrevDetectedActors)
				{
					Prev->EnableOcclusion(false);
					Prev->EnableOutLine(false);
				}

				mPrevDetectedActors.Empty();
			}

			//구해진 해당 마우스 포인터에 멀티 스윕 충돌을 계산한다 
			TArray<FHitResult>	HitArray;

			FCollisionQueryParams	param;
			param.AddIgnoredActor(this);

			bool Collision = GetWorld()->SweepMultiByChannel(HitArray,
				WorldLocation,
				WorldLocation,
				FQuat::Identity,
				ECollisionChannel::ECC_GameTraceChannel9,
				FCollisionShape::MakeSphere(SphereRadius), param);

			//충돌이 존재하면 타깃을 검출하고 어클루전 플래그를 설정한다 
			if (Collision)
			{
				for (auto& Hit : HitArray)
				{
					ICharacterRenderingInterface* Rendering =
						Cast<ICharacterRenderingInterface>(Hit.GetActor());

					if (Rendering)
					{
						//UE_LOG(TestGame, Display, TEXT("Occlusion entered"));
						Rendering->EnableOcclusion(true);
						Rendering->EnableOutLine(true);

						mPrevDetectedActors.Add(Rendering);
					}
				}
			}
		}
	}

	//++mSkill2Count;

	//if (mSkill2Count == 7)
	//{
	//	if (!mPrevDetectedActors.IsEmpty())
	//	{
	//		for (auto& Prev : mPrevDetectedActors)
	//		{
	//			Prev->EnableOcclusion(false);
	//		}

	//		mPrevDetect.Empty();
	//	}

	//	GetWorldTimerManager().ClearTimer(mSkill2Timer);
	//}
}

void ARanger::Skill1Init()
{
	if (mSkill1ProjectileAsset) // Niagara 에셋이 유효한지 확인
	{
		float SphereSize = 300.0f;
		float SpawnHeight = 2000.0f;

		// 랜덤 위치 생성 (X, Y 범위 내에서 무작위 값)
		float RandX = FMath::RandRange(-SphereSize, SphereSize);
		float RandY = FMath::RandRange(-SphereSize, SphereSize);
		float RandZ = SpawnHeight; // Z축 보정 (+값 추가)

		FVector WorldLocation, WorldDirection;
		//플레이어 컨트롤러가 널포인터거나 지움예약/GC 인경우 리턴
		APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		if (!IsValid(PlayerController))
			return;

		if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			// 레이캐스트 수행하여 실제 월드 좌표 얻기
			FHitResult HitResult;
			FVector Start = WorldLocation;
			FVector End = Start + (WorldDirection * 10000.0f); // 적절한 거리 설정

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this); // 캐릭터 제외

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
			{
				WorldLocation = HitResult.Location; // 실제 충돌 지점으로 업데이트

				FVector SpawnPosition = WorldLocation + FVector(RandX, RandY, RandZ);

				//람다 함수로 인자 전달할 수 있게 정의 후  타이머로 등록
				FTimerDelegate TimerDel;
				TimerDel.BindLambda([this, WorldLocation, SphereSize, SpawnHeight]()
					{
						this->Skill1Loop(WorldLocation, SphereSize, SpawnHeight);
					});

				GetWorld()->GetTimerManager().SetTimer(
					mSkillTimer1Loop,
					TimerDel,
					0.033f,
					true,
					0.01f
				);

				if (mSkillTimer1Loop.IsValid())
					UE_LOG(TestGame, Warning, TEXT("mSkillTimer1 was set"));

				// 일정 개수 후 타이머 종료
				FTimerHandle StopTimer;
				GetWorld()->GetTimerManager().SetTimer(
					StopTimer,
					[this]() { GetWorld()->GetTimerManager().ClearTimer(mSkillTimer1Loop); },
					3.0f,
					false // 한 번만 실행
				);
			}
		}
	}
}

void ARanger::Skill1Loop(FVector TargetLocation, float SphereSize, float SpawnHeight)
{
	// 랜덤 위치 생성 (X, Y 범위 내에서 무작위 값)
	float RandX = FMath::RandRange(-SphereSize, SphereSize);
	float RandY = FMath::RandRange(-SphereSize, SphereSize);
	float RandZ = SpawnHeight; // Z축 보정 (+값 추가)

	FVector SpawnPosition = TargetLocation + FVector(RandX, RandY, RandZ);

	// Niagara 파티클 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	ARogueProjectileA* Projectile = GetWorld()->SpawnActor<ARogueProjectileA>(ARogueProjectileA::StaticClass(), SpawnPosition, FRotator::ZeroRotator, SpawnParams);
	if (Projectile)
	{
		FVector Direction = FVector(0, 0, -1); // 아래로 향하게
		Projectile->InitProjectile(Direction, 3000.f); // 속도 부여
	}
}

void ARanger::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}



