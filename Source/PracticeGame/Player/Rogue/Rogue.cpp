// Fill out your copyright notice in the Description page of Project Settings.


#include "Rogue.h"
#include "../../Actor/RogueProjectileA.h"
#include "../../NPC/Monster/Monster.h"

ARogue::ARogue()
{
	mDataKey = TEXT("Rogue");
	/*
	* 메시 및 캡슐 설정 
	*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMesh(TEXT(
			"/Script/Engine.SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));
	if (SkeletalMesh.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(SkeletalMesh.Object);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.5f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.5f);

	//무기 콜리전 박스 생성
	//도적은 쌍수무기를 들고 있다. 두개의 콜리전 박스를 각각 생성한다
	for (int32 i = 0; i < 0; i++)
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
			"/Script/Engine.AnimBlueprint'/Game/Player/Rogue/ABPRogue_T.ABPRogue_T_C'")	);
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

//virtual void RunSweepConeAndDamage(float Damage, float SphereSize, float AddedDistance, float InnerAngle, ECollisionChannel TraceChannel, UNiagaraSystem* VFXAsset = nullptr, bool bSpawnVFXOnHit = true, USoundBase* SFXAsset = nullptr, bool bSpawnSFXOnHit = true)
//{
//}
//
//virtual void RunSweepConeAndDamage(float Damage, float SphereSize, float AddedDistance, float InnerAngle, ECollisionChannel TraceChannel, UParticleSystem* VFXAsset = nullptr, bool bSpawnVFXOnHit = true, USoundBase* SFXAsset = nullptr, bool bSpawnSFXOnHit = true)
//{
//
//}

void ARogue::NormalAttack()
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

void ARogue::SpecialAttack()
{
	AddMP(-5);
	UpdateMPBar();
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
		if(AMonster* Monster = Cast<AMonster>(HitActor	))	
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

void ARogue::CleaveAttack()
{
	GetWorld()->GetTimerManager().SetTimer(
		mCleaveReleaseTimer,
		this,
		&ARogue::ReleaseCleave,
		0.36f,
		false);
}

void ARogue::ReleaseCleave()
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

	//플레이어에 콘모양 스윕 통합하여 그걸로 호출하기로 함 아래는 구코드 남겨놓음
	//float SphereSize = 150.f;			//생성할 구의 크기
	//float AddedDistance = 20.f;		//구의 중심점과 케릭터사이의 거리 추가 보정
	//float InnerAngle = 50.0f;			//조건으로 검출할 각도 (단측면의 값임 총 각도는 * 2 )

	//// 1. 케릭터의 전방 방향을 구하고 각도를 계산할 위치를 잡아준다Reference Point
	//FVector MeshForwardVector = GetMesh()->GetForwardVector();
	////* 메쉬의 Yaw 회전 틀어짐 보정 90f
	//FVector AdjustedForward = MeshForwardVector.RotateAngleAxis(90.0f, FVector(0, 0, 1));
	//FVector ReferencePoint = GetActorLocation() + (AdjustedForward * AddedDistance);

	////구를 생성할 중심점을 만든다   =   SpherePoint
	//FVector SpherePoint = GetActorLocation() + (AdjustedForward * (SphereSize + AddedDistance)); // Sweep 시작 위치
	//FVector SweepEnd = SpherePoint;

	//// 2. 설정한 반경(SphereSize) 크기의 스피어 Sweep 을 실행한다
	//FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereSize);
	//TArray<FHitResult> HitResults;

	//bool bHit = GetWorld()->SweepMultiByChannel(
	//	HitResults,
	//	SpherePoint,														//구를 만들 시작 포인트
	//	SweepEnd,														//종료 포인트 (구체이므로 같은곳을 지정하였다
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel7,		// PlayerWeaponSweep -> 트레이스 채널
	//	SphereShape														//SphereSize 크기의 Sphere 로 만든다
	//);
	//if (CVarDebugLines.GetValueOnGameThread() == 1) // 값이 1이면 디버그 출력
	//{

	//// 스피어 디버그 드로잉 (Sweep 검사 범위)
	//float DrawTime = 2.0f;
	//DrawDebugSphere(
	//	GetWorld(),
	//	SpherePoint,					// 구체 중심
	//	SphereSize,					// 반지름
	//	12,								// 세그먼트 수 (디테일 정도)
	//	FColor::Red,					// 색상
	//	false,
	//	DrawTime 						// 지속 시간
	//);

	//// 전방 기준점 디버그 드로잉
	//DrawDebugPoint(
	//	GetWorld(),
	//	ReferencePoint,
	//	10.0f,							// 크기
	//	FColor::Blue,
	//	false,
	//	DrawTime
	//);

	//// Cone 판정 영역 디버그 드로잉 (각도 표시)
	//FVector LeftLimit = AdjustedForward.RotateAngleAxis(-InnerAngle, FVector(0, 0, 1));			// -각도 방향(좌측각)
	//FVector RightLimit = AdjustedForward.RotateAngleAxis(InnerAngle, FVector(0, 0, 1));			// +각도 방향(우측각)

	//DrawDebugLine(GetWorld(), ReferencePoint, ReferencePoint + LeftLimit * SphereSize, FColor::Green, false, DrawTime);
	//DrawDebugLine(GetWorld(), ReferencePoint, ReferencePoint + RightLimit * SphereSize, FColor::Green, false, DrawTime);
	//} 

	////최적화 : 충돌 없으면 나가기
	//if (!bHit)
	//	return;

	//// 3. 모든 충돌체 탐색
	//for (const FHitResult& Hit : HitResults)
	//{
	//	AActor* HitActor = Hit.GetActor();
	//	//최적화 : 충돌 가능한 엑터가 아니면 건너뛰기 
	//	if (!HitActor)
	//		continue;

	//	// 4. 기준 지점(ReferencePoint)에서 충돌체 방향을 계산 (정규화로 정확도 보장)
	//	FVector ToHitActor = (HitActor->GetActorLocation() - ReferencePoint).GetSafeNormal();
	//	// 내적구하기 ForwardVector vs ToHitActor
	//	float DotProduct = FVector::DotProduct(AdjustedForward, ToHitActor);

	//	//비교 방식 :  두 값(내적 vs 각도) 을 비교가능하도록 같은 성질로 만들어야 한다
	//	// 방식 1. 라디안 DotProduct 을 각도로 변환하기 (역코사인)
	//	float Angle = FMath::Acos(DotProduct) * (180.0f / PI);
	//	// 방식 2. 각도 InnerAngle 을 라디안으로 변환하기 (코사인)
	//	//			float TargetRadian = FMath::Cos(FMath::DegreesToRadians(InnerAngle));
	//	//판정 조건에 의해 검출된 엑터에 대한 구현 
	//	//목표가 cone 모양라면 음수를 걸러내야햔다 (음수는 케릭터 뒷방향이므로..)
	//	if (Angle <= InnerAngle && Angle >= 0)
	//	{
	//		//
	//		//파티클 생성 : 비동기방식 런타임 로드  
	//		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mNiagaraCleaveHit, Hit.ImpactPoint);

	//		UE_LOG(TestGame, Warning, TEXT("Actor [%s] has in inside of %0.2f degrees"), *HitActor->GetName(), InnerAngle);
	//	}
	//}

	////사운드생성 : 비동기방식- 런타임 로드
	//USoundBase* HitSound = LoadObject<USoundBase>(
	//	GetWorld(),
	//	TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_impact_armor_metal.sfx_impact_armor_metal'"));

	//UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound,
	//	GetMesh()->GetComponentLocation(), FRotator::ZeroRotator,
	//	2.f);
}

void ARogue::MakePreviewCastZone() 
{
	UE_LOG(TestGame, Warning, TEXT("Entered MakePreviewCastZone"));
	//영역표기를 위해 주기적으로 업데이트하는 함수를 호출한다.
	GetWorld()->GetTimerManager().SetTimer(
		mSkillTimer1,
		this,
		&ARogue::UpdateSkill1Selection,
		0.05f,
		true,
		0.01f);
	if(mSkillTimer1.IsValid())
		UE_LOG(TestGame, Warning, TEXT("mSkillTimer1 was set"));
}

void ARogue::CancelPreviewCastZone()
{
	DeleteSkill1Decal();
	if(mSkillTimer1.IsValid())
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

void ARogue::AcceptPreviewCastZone()
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

void ARogue::UpdateSkill1Selection()
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

void ARogue::Skill1Init()
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

				//개발 타이머 핸들 포인터 생성
				TSharedPtr<FTimerHandle> RepeatThisSkillTimer = MakeShared<FTimerHandle>();

				//람다 함수로 인자 전달할 수 있게 정의 후  타이머로 등록
				FTimerDelegate TimerDel;
				TimerDel.BindLambda([this, WorldLocation, SphereSize, SpawnHeight]()
					{
						this->Skill1Loop(WorldLocation, SphereSize, SpawnHeight);
					});

				GetWorld()->GetTimerManager().SetTimer(
					*RepeatThisSkillTimer,
					TimerDel,
					0.033f,
					true,
					0.01f
				);

				if (mSkillTimer1Loop.IsValid())
					UE_LOG(TestGame, Warning, TEXT("mSkillTimer1 was set"));

				// 종료 타이머
				FTimerHandle StopTimer;
				GetWorld()->GetTimerManager().SetTimer(
					StopTimer,
					[this, RepeatThisSkillTimer]()
					{
						if (RepeatThisSkillTimer.IsValid())
						{
							GetWorld()->GetTimerManager().ClearTimer(*RepeatThisSkillTimer);
						}
					},
					3.0f,
					false
				);
			}
		}
	}
	float SkillCost = -10.0f;
	//마나를 차감한다
	AddMP(SkillCost);
	//차감한 마나를 HUD 에 반영한다 
	UpdateMPBar();
}

void ARogue::Skill1Loop(FVector TargetLocation, float SphereSize, float SpawnHeight)
{
	// 랜덤 위치 생성 (X, Y 범위 내에서 무작위 값)
	float RandX = FMath::RandRange(-SphereSize, SphereSize);
	float RandY = FMath::RandRange(-SphereSize, SphereSize);
	float RandZ = SpawnHeight; // Z축 보정 (+값 추가)

	FVector SpawnPosition = TargetLocation + FVector(RandX, RandY, RandZ);

	// Niagara 파티클 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = this;   //행위자 설정 (나중에 다른 클래스에서 추적할 떄 중요함
	SpawnParams.Owner = this; // 소유자 설정 (나중에 다른 클래스에서 추적할 떄 중요함=

	ARogueProjectileA* Projectile = GetWorld()->SpawnActor<ARogueProjectileA>(ARogueProjectileA::StaticClass(), SpawnPosition, FRotator::ZeroRotator, SpawnParams);
	if (Projectile)
	{
		FVector Direction = FVector(0, 0, -1); // 아래로 향하게
		Projectile->InitProjectile(Direction, 3000.f); // 속도 부여
	}
}

void ARogue::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}