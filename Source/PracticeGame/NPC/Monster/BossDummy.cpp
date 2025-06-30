// Fill out your copyright notice in the Description page of Project Settings.


#include "BossDummy.h"
#include "MonsterController.h"
#include "../../Player/PlayerCharacter.h"

ABossDummy::ABossDummy()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT(
			"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Prime_Helix/Meshes/Prime_Helix.Prime_Helix'"
		));

	if (MeshAsset.Succeeded())
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);

	//데이터셋에서 정보를 가져오기 위해 정확한 문자열 지정이 필요함 
	mDataKey = TEXT("BossDummy01");

	mBody->SetCapsuleHalfHeight(200.f);
	//부모에서 돌려놓음
	//mMesh->SetRelativeRotation(FRotator(0, -90, 0));
	mMesh->SetRelativeLocation(FVector(0, 0, -200));
	mBody->SetCapsuleRadius(134.0f);

	//애님인스턴스 (블루프린트 만들어서 할당해야 함
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Monster/ABPBossDummy.ABPBossDummy_C'"));

	if (AnimAsset.Succeeded())
		mMesh->SetAnimInstanceClass(AnimAsset.Class);
}

void ABossDummy::BeginPlay()
{
	Super::BeginPlay();

}

void ABossDummy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//데미지를 즉시 입히기 때문에 특정 프레임에서 호출하는 것이 적당할 수 있다
void ABossDummy::NormalAttack()
{
	AAIController* AI = GetController<AAIController>();

	USoundBase*	GroundSlamSFX = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sound-design-elements-impact-sfx-ps-059-333119.sound-design-elements-impact-sfx-ps-059-333119'"));
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GroundSlamSFX, mMesh->GetComponentLocation());
	if (IsValid(AI))
	{
		AActor* Target = Cast<AActor>(AI->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

		if (IsValid(Target))
		{
			FDamageEvent	DmgEvent;

			//데미지를 입힌다
			Target->TakeDamage(mAttack, DmgEvent, AI, this);
		}
	}
}

//데미지를 입히기 때문에 사거리 안에 있을때만 호출하던지 
// 데미지를 입히기전에 조건을 검사해야 한다
void ABossDummy::SpecialAttack()
{
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

			//데미지를 입힌다 기본 공격의 5 배로 설정한다
			Target->TakeDamage(mAttack * 5.0f, DmgEvent, AI, this);
			
			APlayerCharacter* PC = Cast<APlayerCharacter>(Target);
			//타깃이 플레이어 캐릭터 클래스라면 VFX 를 입힌다
			if(IsValid(PC))
			{
			//대상은 본  "Chest"
				FVector	VFXLocation = PC->GetMesh()->GetSocketLocation(TEXT("Chest"));

				//if (!IsValid(GetWorld()))
				//{
				//	// 안전하게 리턴 또는 대체 처리
				//	return;
				//}
			UParticleSystem* HitParticle = LoadObject<UParticleSystem>(
				GetWorld(),
				TEXT("/Script/Engine.ParticleSystem'/Game/ParagonSparrow/FX/Particles/Sparrow/Abilities/Primary/FX/P_PROTO_Proto_Ballistic_HitPlayer.P_PROTO_Proto_Ballistic_HitPlayer'"));

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				HitParticle, VFXLocation,
				FRotator::ZeroRotator);
			}
		}
	}
}

