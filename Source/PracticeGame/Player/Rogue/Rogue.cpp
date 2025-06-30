// Fill out your copyright notice in the Description page of Project Settings.


#include "Rogue.h"
#include "../../Actor/RogueProjectileA.h"
#include "../../NPC/Monster/Monster.h"

ARogue::ARogue()
{
	mDataKey = TEXT("Rogue");
	/*
	* �޽� �� ĸ�� ���� 
	*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SkeletalMesh(TEXT(
			"/Script/Engine.SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));
	if (SkeletalMesh.Succeeded())
		GetMesh()->SetSkeletalMeshAsset(SkeletalMesh.Object);

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90.5f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.5f);

	//���� �ݸ��� �ڽ� ����
	//������ �ּ����⸦ ��� �ִ�. �ΰ��� �ݸ��� �ڽ��� ���� �����Ѵ�
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
		
		//����� ��ü�� �Ҵ��Ѵ�
		mWeaponColBox.Add(NewBox);
	}

	//JumpZVelocity ���� ���̸� �����Ѵ�
	GetCharacterMovement()
		->JumpZVelocity = 600.f;
	//GravityScale  �߷��� �����Ѵ�
	GetCharacterMovement()
		->GravityScale = 1.0f;
	//AirControl  ���� �̵� ��� �����Ѵ�, 
	// ���� ���� �̵��� ���ѵǰ�, �������� ���� �̵��� �����ο����� 
	GetCharacterMovement()
		->AirControl = 0.5f;

	//JumpMaxCount ��  �����ϱ� ������ �ִ� ������ ���� Ƚ���� �����Ѵ�
	JumpMaxCount = 2;

	//	�ִϸ��̼� �������Ʈ ���� �ε�
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		ABPAsset(TEXT(
			"/Script/Engine.AnimBlueprint'/Game/Player/Rogue/ABPRogue_T.ABPRogue_T_C'")	);
	if (ABPAsset.Succeeded())
		GetMesh()->SetAnimInstanceClass(ABPAsset.Class);

	//����ü ���·ε�
	static ConstructorHelpers::FClassFinder<AATestProjectile> TestProjectile(TEXT(
		"/Script/CoreUObject.Class'/Script/PracticeGame.ATestProjectile'"));
	if (TestProjectile.Succeeded())
		mProjectileClass = TestProjectile.Class;

	//�����̾� ������ (��ų�� �� ��
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> AssetVampireSlash(
		TEXT("/Script/Niagara.NiagaraSystem'/Game/MixedVFX/Particles/Slashes/NS_VampireSlash.NS_VampireSlash'"));

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> CleaveVFXAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/MixedVFX/Particles/Slashes/SeparateParts/Hits/NS_VampireSlash_Hit.NS_VampireSlash_Hit'"));
	if (CleaveVFXAsset.Succeeded())
		mNiagaraCleaveHit = CleaveVFXAsset.Object;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> Skill1ProjectileAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/MixedVFX/Particles/Projectiles/NS_Projectile_04.NS_Projectile_04'"));
	if (Skill1ProjectileAsset.Succeeded())
		mSkill1ProjectileAsset = Skill1ProjectileAsset.Object;

	//���̾ư������� ��ƼŬ���� ����
	mTrailAssetMode = ETrailAssetMode::NIAGARASYSTEM;

	if (mTrailAssetMode == ETrailAssetMode::NIAGARASYSTEM)
	{
		//Ʈ���� ��ƼŬ �ε� (�޼� ������ ����)
		static ConstructorHelpers::FObjectFinder<UNiagaraSystem> LeftTrailAssetNiagara(
			TEXT("/Script/Niagara.NiagaraSystem'/Game/Player/Animation/FX/NS_weaponTrail.NS_weaponTrail'"));

		//���� �ٸ� ������ ��� �ϴ� ���
		// static ConstructorHelpers::FObjectFinder<UNiagaraComponent> RightTrailAssetNiagara(
		// TEXT(""));

		if (LeftTrailAssetNiagara.Succeeded())
		{
			mTrailEffectNiagaraL = LeftTrailAssetNiagara.Object;
			mTrailEffectNiagaraR = LeftTrailAssetNiagara.Object;
		}
		//���� �ٸ� ������ ��� �ϴ� ���
		/*if (RightTrailAssetNiagara.Succeeded())
		{
			mTrailEffectR = RightTrailAssetNiagara.Object;
		}*/
	}
	else if (mTrailAssetMode == ETrailAssetMode::PARTICLESYSTEM)
	{
		//Ʈ���� ��ƼŬ �ε� (�޼� ������ ����)
		static ConstructorHelpers::FObjectFinder<UParticleSystem> LeftTrailAsset(
			TEXT("/Script/Engine.MaterialInstanceConstant'/Game/MixedVFX/Materials/MaterialInstances/MI_Slash_01.MI_Slash_01'"));
		//���� �ٸ� ������ ��� �ϴ� ���
		// static ConstructorHelpers::FObjectFinder<UParticleSystem> RightTrailAsset(
		// TEXT(""));

		if (LeftTrailAsset.Succeeded())
		{
			mTrailEffectL = LeftTrailAsset.Object;
			mTrailEffectR = LeftTrailAsset.Object;
		}
		//���� �ٸ� ������ ��� �ϴ� ���
		/*if (RightTrailAsset.Succeeded())
		{
			mTrailEffectR = RightTrailAsset.Object;
		}*/
	}

	//�����ڿ��� �ε��� ��Į��
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
	//������ ����� �� 
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

		//���� ������ �������� Cone ���� ó���� �Ѵ� ->  �� ���� -> ���ǿ�����(����) ���� -> ����
		RunSweepConeAndDamage(DamagePoint, SphereSize, SphereDistance, -AngleStartDistance, InnerAngle, ECollisionChannel::ECC_GameTraceChannel7, VFXAsset, true, SFXAsset, true);
	}
}

void ARogue::SpecialAttack()
{
	AddMP(-5);
	UpdateMPBar();
	//������� : �񵿱���- ��Ÿ�� �ε�
	USoundBase* LaunchSFX = LoadObject<USoundBase>(
		GetWorld(),
		TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_boom01.sfx_boom01'"));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), LaunchSFX,
		GetMesh()->GetComponentLocation(), FRotator::ZeroRotator,
		2.f);

	float SphereSize = 400.f;			//������ ���� ũ��
	
	// 1. �ɸ����� ���� ������ ���ϰ� ������ ����� ��ġ�� ����ش�Reference Point
	FVector SpherePoint = GetActorLocation(); // Sweep ���� ��ġ
	FVector SweepEnd = SpherePoint;

	// 2. ������ �ݰ�(SphereSize) ũ���� ���Ǿ� Sweep �� �����Ѵ�
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereSize);
	TArray<FHitResult> HitResults;

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		SpherePoint,														//���� ���� ���� ����Ʈ
		SweepEnd,														//���� ����Ʈ (��ü�̹Ƿ� �������� �����Ͽ���
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel7,		// PlayerWeaponSweep -> Ʈ���̽� ä��
		SphereShape														//SphereSize ũ���� Sphere �� �����
	);
	if (CVarDebugLines.GetValueOnGameThread() == 1) // ���� 1�̸� ����� ���
	{

		// ���Ǿ� ����� ����� (Sweep �˻� ����)
		float DrawTime = 2.0f;
		DrawDebugSphere(
			GetWorld(),
			SpherePoint,					// ��ü �߽�
			SphereSize,					// ������
			12,								// ���׸�Ʈ �� (������ ����)
			FColor::Red,					// ����
			false,
			DrawTime 						// ���� �ð�
		);
	}
	//����ȭ : �浹 ������ ������
	if (!bHit)
		return;

	// 3. ��� �浹ü Ž��
	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		//����ȭ : �浹 ������ ���Ͱ� �ƴϸ� �ǳʶٱ� 
		if (!HitActor)
			continue;

		//���� �� ���� Ŭ���� �� ��쿡 �浹 ó��
		if(AMonster* Monster = Cast<AMonster>(HitActor	))	
		{
			//��ƼŬ ���� : �񵿱��� ��Ÿ�� �ε�  
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

	//���� ������ �������� Cone ���� ó���� �Ѵ� ->  �� ���� -> ���ǿ�����(����) ���� -> ����
	RunSweepConeAndDamage(DamagePoint, SphereSize, SphereDistance, -AngleStartDistance, InnerAngle, ECollisionChannel::ECC_GameTraceChannel7, VFXAsset, true, SFXAsset, true);

	//�÷��̾ �ܸ�� ���� �����Ͽ� �װɷ� ȣ���ϱ�� �� �Ʒ��� ���ڵ� ���ܳ���
	//float SphereSize = 150.f;			//������ ���� ũ��
	//float AddedDistance = 20.f;		//���� �߽����� �ɸ��ͻ����� �Ÿ� �߰� ����
	//float InnerAngle = 50.0f;			//�������� ������ ���� (�������� ���� �� ������ * 2 )

	//// 1. �ɸ����� ���� ������ ���ϰ� ������ ����� ��ġ�� ����ش�Reference Point
	//FVector MeshForwardVector = GetMesh()->GetForwardVector();
	////* �޽��� Yaw ȸ�� Ʋ���� ���� 90f
	//FVector AdjustedForward = MeshForwardVector.RotateAngleAxis(90.0f, FVector(0, 0, 1));
	//FVector ReferencePoint = GetActorLocation() + (AdjustedForward * AddedDistance);

	////���� ������ �߽����� �����   =   SpherePoint
	//FVector SpherePoint = GetActorLocation() + (AdjustedForward * (SphereSize + AddedDistance)); // Sweep ���� ��ġ
	//FVector SweepEnd = SpherePoint;

	//// 2. ������ �ݰ�(SphereSize) ũ���� ���Ǿ� Sweep �� �����Ѵ�
	//FCollisionShape SphereShape = FCollisionShape::MakeSphere(SphereSize);
	//TArray<FHitResult> HitResults;

	//bool bHit = GetWorld()->SweepMultiByChannel(
	//	HitResults,
	//	SpherePoint,														//���� ���� ���� ����Ʈ
	//	SweepEnd,														//���� ����Ʈ (��ü�̹Ƿ� �������� �����Ͽ���
	//	FQuat::Identity,
	//	ECollisionChannel::ECC_GameTraceChannel7,		// PlayerWeaponSweep -> Ʈ���̽� ä��
	//	SphereShape														//SphereSize ũ���� Sphere �� �����
	//);
	//if (CVarDebugLines.GetValueOnGameThread() == 1) // ���� 1�̸� ����� ���
	//{

	//// ���Ǿ� ����� ����� (Sweep �˻� ����)
	//float DrawTime = 2.0f;
	//DrawDebugSphere(
	//	GetWorld(),
	//	SpherePoint,					// ��ü �߽�
	//	SphereSize,					// ������
	//	12,								// ���׸�Ʈ �� (������ ����)
	//	FColor::Red,					// ����
	//	false,
	//	DrawTime 						// ���� �ð�
	//);

	//// ���� ������ ����� �����
	//DrawDebugPoint(
	//	GetWorld(),
	//	ReferencePoint,
	//	10.0f,							// ũ��
	//	FColor::Blue,
	//	false,
	//	DrawTime
	//);

	//// Cone ���� ���� ����� ����� (���� ǥ��)
	//FVector LeftLimit = AdjustedForward.RotateAngleAxis(-InnerAngle, FVector(0, 0, 1));			// -���� ����(������)
	//FVector RightLimit = AdjustedForward.RotateAngleAxis(InnerAngle, FVector(0, 0, 1));			// +���� ����(������)

	//DrawDebugLine(GetWorld(), ReferencePoint, ReferencePoint + LeftLimit * SphereSize, FColor::Green, false, DrawTime);
	//DrawDebugLine(GetWorld(), ReferencePoint, ReferencePoint + RightLimit * SphereSize, FColor::Green, false, DrawTime);
	//} 

	////����ȭ : �浹 ������ ������
	//if (!bHit)
	//	return;

	//// 3. ��� �浹ü Ž��
	//for (const FHitResult& Hit : HitResults)
	//{
	//	AActor* HitActor = Hit.GetActor();
	//	//����ȭ : �浹 ������ ���Ͱ� �ƴϸ� �ǳʶٱ� 
	//	if (!HitActor)
	//		continue;

	//	// 4. ���� ����(ReferencePoint)���� �浹ü ������ ��� (����ȭ�� ��Ȯ�� ����)
	//	FVector ToHitActor = (HitActor->GetActorLocation() - ReferencePoint).GetSafeNormal();
	//	// �������ϱ� ForwardVector vs ToHitActor
	//	float DotProduct = FVector::DotProduct(AdjustedForward, ToHitActor);

	//	//�� ��� :  �� ��(���� vs ����) �� �񱳰����ϵ��� ���� ������ ������ �Ѵ�
	//	// ��� 1. ���� DotProduct �� ������ ��ȯ�ϱ� (���ڻ���)
	//	float Angle = FMath::Acos(DotProduct) * (180.0f / PI);
	//	// ��� 2. ���� InnerAngle �� �������� ��ȯ�ϱ� (�ڻ���)
	//	//			float TargetRadian = FMath::Cos(FMath::DegreesToRadians(InnerAngle));
	//	//���� ���ǿ� ���� ����� ���Ϳ� ���� ���� 
	//	//��ǥ�� cone ����� ������ �ɷ������h�� (������ �ɸ��� �޹����̹Ƿ�..)
	//	if (Angle <= InnerAngle && Angle >= 0)
	//	{
	//		//
	//		//��ƼŬ ���� : �񵿱��� ��Ÿ�� �ε�  
	//		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mNiagaraCleaveHit, Hit.ImpactPoint);

	//		UE_LOG(TestGame, Warning, TEXT("Actor [%s] has in inside of %0.2f degrees"), *HitActor->GetName(), InnerAngle);
	//	}
	//}

	////������� : �񵿱���- ��Ÿ�� �ε�
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
	//����ǥ�⸦ ���� �ֱ������� ������Ʈ�ϴ� �Լ��� ȣ���Ѵ�.
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

	//���̶���Ʈ�� �༮�� �����ֱ�
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
	//���̶���Ʈ�� �༮�� �����ֱ�
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

	//�÷��̾� ��Ʈ�ѷ��� �������Ͱų� ���򿹾�/GC �ΰ�� ����
	APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	if (!IsValid(PlayerController))
		return;

	FVector WorldLocation, WorldDirection;
	if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		// ����ĳ��Ʈ �����Ͽ� ���� ���� ��ǥ ���
		FHitResult HitResult;
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f); // ������ �Ÿ� ����

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // ĳ���� ����

		if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
		{
			WorldLocation = HitResult.Location; // ���� �浹 �������� ������Ʈ

			if (CVarDebugLines.GetValueOnGameThread() == 1) // ���� 1�̸� ����� ���
			{
				// ���Ǿ� ����� ����� (Sweep �˻� ����)
				float DrawTime = 0.05f;
				DrawDebugSphere(
					GetWorld(),
					WorldLocation,					// ��ü �߽�
					SphereRadius,								// ������
					12,								// ���׸�Ʈ �� (������ ����)
					FColor::Green,					// ����
					false,
					DrawTime 						// ���� �ð�
				);
			}

			//��Į�� �����Ѵ� -> �ش� �Լ��� ������ ��ġ�� ������Ʈ�Ѵ�
			SpawnSkill1DecalAtLocation(WorldLocation, FVector(SphereRadius, SphereRadius, SphereRadius));

			////��Į�� �����Ѵ�(���� �ε�)
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
			//		Decal->SetFadeScreenSize(0.1f);  // ���� �Ÿ����� ���̵� �ƿ�
			//		Decal->SetWorldLocation(WorldLocation);
			//	}

			//UE_LOG(TestGame, Warning, TEXT("UpdateSkill1Selection : succeeded collision check"));

			// ������ �ε����� ����� ��� ��Ȱ��ȭ�Ѵ�.
			if (!mPrevDetectedActors.IsEmpty())
			{
				for (auto& Prev : mPrevDetectedActors)
				{
					Prev->EnableOcclusion(false);
					Prev->EnableOutLine(false);
				}

				mPrevDetectedActors.Empty();
			}

			//������ �ش� ���콺 �����Ϳ� ��Ƽ ���� �浹�� ����Ѵ� 
			TArray<FHitResult>	HitArray;

			FCollisionQueryParams	param;
			param.AddIgnoredActor(this);

			bool Collision = GetWorld()->SweepMultiByChannel(HitArray,
				WorldLocation,
				WorldLocation,
				FQuat::Identity,
				ECollisionChannel::ECC_GameTraceChannel9,
				FCollisionShape::MakeSphere(SphereRadius), param);

			//�浹�� �����ϸ� Ÿ���� �����ϰ� ��Ŭ���� �÷��׸� �����Ѵ� 
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
	if (mSkill1ProjectileAsset) // Niagara ������ ��ȿ���� Ȯ��
	{
		float SphereSize = 300.0f;
		float SpawnHeight = 2000.0f;

		// ���� ��ġ ���� (X, Y ���� ������ ������ ��)
		float RandX = FMath::RandRange(-SphereSize, SphereSize);
		float RandY = FMath::RandRange(-SphereSize, SphereSize);
		float RandZ = SpawnHeight; // Z�� ���� (+�� �߰�)

		FVector WorldLocation, WorldDirection;
		//�÷��̾� ��Ʈ�ѷ��� �������Ͱų� ���򿹾�/GC �ΰ�� ����
		APlayerController* PlayerController = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
		if (!IsValid(PlayerController))
			return;

		if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			// ����ĳ��Ʈ �����Ͽ� ���� ���� ��ǥ ���
			FHitResult HitResult;
			FVector Start = WorldLocation;
			FVector End = Start + (WorldDirection * 10000.0f); // ������ �Ÿ� ����

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this); // ĳ���� ����

			if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
			{
				WorldLocation = HitResult.Location; // ���� �浹 �������� ������Ʈ

				FVector SpawnPosition = WorldLocation + FVector(RandX, RandY, RandZ);

				//���� Ÿ�̸� �ڵ� ������ ����
				TSharedPtr<FTimerHandle> RepeatThisSkillTimer = MakeShared<FTimerHandle>();

				//���� �Լ��� ���� ������ �� �ְ� ���� ��  Ÿ�̸ӷ� ���
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

				// ���� Ÿ�̸�
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
	//������ �����Ѵ�
	AddMP(SkillCost);
	//������ ������ HUD �� �ݿ��Ѵ� 
	UpdateMPBar();
}

void ARogue::Skill1Loop(FVector TargetLocation, float SphereSize, float SpawnHeight)
{
	// ���� ��ġ ���� (X, Y ���� ������ ������ ��)
	float RandX = FMath::RandRange(-SphereSize, SphereSize);
	float RandY = FMath::RandRange(-SphereSize, SphereSize);
	float RandZ = SpawnHeight; // Z�� ���� (+�� �߰�)

	FVector SpawnPosition = TargetLocation + FVector(RandX, RandY, RandZ);

	// Niagara ��ƼŬ ����
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnParams.Instigator = this;   //������ ���� (���߿� �ٸ� Ŭ�������� ������ �� �߿���
	SpawnParams.Owner = this; // ������ ���� (���߿� �ٸ� Ŭ�������� ������ �� �߿���=

	ARogueProjectileA* Projectile = GetWorld()->SpawnActor<ARogueProjectileA>(ARogueProjectileA::StaticClass(), SpawnPosition, FRotator::ZeroRotator, SpawnParams);
	if (Projectile)
	{
		FVector Direction = FVector(0, 0, -1); // �Ʒ��� ���ϰ�
		Projectile->InitProjectile(Direction, 3000.f); // �ӵ� �ο�
	}
}

void ARogue::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}