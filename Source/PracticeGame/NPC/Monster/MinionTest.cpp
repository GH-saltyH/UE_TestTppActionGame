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

	//�����ͼ¿��� ������ �������� ���� ��Ȯ�� ���ڿ� ������ �ʿ��� 
	mDataKey = TEXT("MinionDummy01");

	mBody->SetCapsuleHalfHeight(63.f);
	//�θ𿡼� ��������
	//mMesh->SetRelativeRotation(FRotator(0, -90, 0));
	mMesh->SetRelativeLocation(FVector(0, 0, -63));

	//�ִ��ν��Ͻ� (�������Ʈ ���� �Ҵ��ؾ� ��
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

//�������� ��� ������ ������ Ư�� �����ӿ��� ȣ���ϴ� ���� ������ �� �ִ�
void AMinionTest::NormalAttack()
{
	
	//���� ���� SFX
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

			//�������� ������
			Target->TakeDamage(mAttack, DmgEvent, AI, this);

			APlayerCharacter* PC = Cast<APlayerCharacter>(Target);
			//Ÿ���� �÷��̾� ĳ���� Ŭ������� VFX �� ������
			if (IsValid(PC))
			{
				//����� ��  "Chest"
				FVector	VFXLocation = PC->GetMesh()->GetSocketLocation(TEXT("Chest"));

				UParticleSystem* HitParticle = LoadObject<UParticleSystem>(
					GetWorld(),
					TEXT("/Script/Engine.ParticleSystem'/Game/ParagonTerra/FX/Particles/Terra/Abilities/Shield/FX/P_Terra_ShieldBlockHit.P_Terra_ShieldBlockHit'"));

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					HitParticle, VFXLocation,
					FRotator::ZeroRotator);

				//���� ��Ʈ ���� 
				USoundBase* ImpactSFX = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/axe-hit-flesh-02-266299.axe-hit-flesh-02-266299'"));

				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSFX, VFXLocation);
			}
		}
	}
}
