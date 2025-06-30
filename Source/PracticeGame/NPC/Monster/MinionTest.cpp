// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionTest.h"
#include "MonsterController.h"
#include "../../Player/PlayerCharacter.h"

AMinionTest::AMinionTest()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT(/*"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Siege_Core_Dusk.Siege_Core_Dusk'"
		));*/
			"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/White_Camp_Minion/Meshes/Minion_melee.Minion_melee'"
		));

	if (MeshAsset.Succeeded())
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);

	//데이터셋에서 정보를 가져오기 위해 정확한 문자열 지정이 필요함 
	mDataKey = TEXT("MinionDummy01");

	mBody->SetCapsuleHalfHeight(63.f);
	//부모에서 돌려놓음
	//mMesh->SetRelativeRotation(FRotator(0, -90, 0));
	mMesh->SetRelativeLocation(FVector(0, 0, -63));

	//애님인스턴스 (블루프린트 만들어서 할당해야 함
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Monster/ABPMinionDummy01.ABPMinionDummy01_C'"));

	if (AnimAsset.Succeeded())
		mMesh->SetAnimInstanceClass(AnimAsset.Class);
}

void AMinionTest::BeginPlay()
{
	Super::BeginPlay();

}

void AMinionTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//데미지를 즉시 입히기 때문에 특정 프레임에서 호출하는 것이 적당할 수 있다
void AMinionTest::NormalAttack()
{
	
	//무기 스윙 SFX
	USoundBase* FireSFX = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_swingwhooshweapon5.sfx_swingwhooshweapon5'"));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSFX, mMesh->GetSocketLocation(TEXT("Muzzle_Front")));

	AAIController* AI = GetController<AAIController>();

	if (IsValid(AI))
	{
		UObject* Obj = (AI->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

		AActor* Target = nullptr;

		if (Obj)
			Target = Cast<AActor>(Obj);

		if (IsValid(Target))
		{
			FDamageEvent	DmgEvent;

			//데미지를 입힌다
			Target->TakeDamage(mAttack, DmgEvent, AI, this);

			APlayerCharacter* PC = Cast<APlayerCharacter>(Target);
			//타깃이 플레이어 캐릭터 클래스라면 VFX 를 입힌다
			if (IsValid(PC))
			{
				//대상은 본  "Chest"
				FVector	VFXLocation = PC->GetMesh()->GetSocketLocation(TEXT("Chest"));

				UParticleSystem* HitParticle = LoadObject<UParticleSystem>(
					GetWorld(),
					TEXT("/Script/Engine.ParticleSystem'/Game/ParagonTerra/FX/Particles/Terra/Abilities/Shield/FX/P_Terra_ShieldBlockHit.P_Terra_ShieldBlockHit'"));

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					HitParticle, VFXLocation,
					FRotator::ZeroRotator);

				//무기 히트 사운드 
				USoundBase* ImpactSFX = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/axe-hit-flesh-02-266299.axe-hit-flesh-02-266299'"));

				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSFX, VFXLocation);
			}
		}
	}
}
