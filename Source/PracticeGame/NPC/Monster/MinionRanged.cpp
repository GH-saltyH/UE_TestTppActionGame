// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionRanged.h"
#include "MonsterController.h"
#include "../../Player/PlayerCharacter.h"

AMinionRanged::AMinionRanged()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Ranged_Dawn.Minion_Lane_Ranged_Dawn'"));
			/*
			* 파란색
			"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Siege_Core_Dawn.Minion_Lane_Siege_Core_Dawn'"));
			/*
			검정색 -> 공격 모션 안나가는 버그 있음
			"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Siege_Dusk.Minion_Lane_Siege_Dusk'"
		));*/
			/*
			검정색
			"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Siege_Core_Dusk.Siege_Core_Dusk'"
		));*/

	if (MeshAsset.Succeeded())
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);

	//데이터셋에서 정보를 가져오기 위해 정확한 문자열 지정이 필요함 
	mDataKey = TEXT("MinionRanged");

	mBody->SetCapsuleHalfHeight(110.5f);
	//부모에서 돌려놓음
	//mMesh->SetRelativeRotation(FRotator(0, -90, 0));
	mMesh->SetRelativeLocation(FVector(0, 0, -110.5));

	//애님인스턴스 (블루프린트 만들어서 할당해야 함
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Monster/ABPMinionRanged.ABPMinionRanged_C'"));

	if (AnimAsset.Succeeded())
		mMesh->SetAnimInstanceClass(AnimAsset.Class);
}

void AMinionRanged::BeginPlay()
{
	Super::BeginPlay();

}

void AMinionRanged::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//데미지를 즉시 입히기 때문에 특정 프레임에서 호출하는 것이 적당할 수 있다
void AMinionRanged::NormalAttack()
{
	AAIController* AI = GetController<AAIController>();

	if (IsValid(AI))
	{
		UObject* Obj = (AI->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

		AActor* Target = nullptr;

		if (Obj)
			Target = Cast<AActor>(Obj);
		
		//발사 사운드
		USoundBase* FireSFX = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_scifi_weapon_shoot_firing_pulse_dn_05.sfx_scifi_weapon_shoot_firing_pulse_dn_05'"));

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSFX, mMesh->GetSocketLocation(TEXT("Muzzle_Front")));
		
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
					TEXT("/Script/Engine.ParticleSystem'/Game/ParagonMinions/FX/Particles/Minions/Prime_Helix/Abilities/PrimaryAttack/FX/Helix_PrimaryImpact.Helix_PrimaryImpact'"));
				FVector	Scale(0.5f, 0.5f, 0.5f);
				FTransform Transform(FRotator::ZeroRotator, VFXLocation, Scale);
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					HitParticle, Transform);
					//FRotator::ZeroRotator);
			}
		}
	}
}
