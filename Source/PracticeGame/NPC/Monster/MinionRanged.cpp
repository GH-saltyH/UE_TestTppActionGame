// Fill out your copyright notice in the Description page of Project Settings.


#include "MinionRanged.h"
#include "MonsterController.h"
#include "../../Player/PlayerCharacter.h"

AMinionRanged::AMinionRanged()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		MeshAsset(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Ranged_Dawn.Minion_Lane_Ranged_Dawn'"));
			/*
			* �Ķ���
			"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Down_Minions/Meshes/Minion_Lane_Siege_Core_Dawn.Minion_Lane_Siege_Core_Dawn'"));
			/*
			������ -> ���� ��� �ȳ����� ���� ����
			"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Minion_Lane_Siege_Dusk.Minion_Lane_Siege_Dusk'"
		));*/
			/*
			������
			"/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Minions/Dusk_Minions/Meshes/Siege_Core_Dusk.Siege_Core_Dusk'"
		));*/

	if (MeshAsset.Succeeded())
		mMesh->SetSkeletalMeshAsset(MeshAsset.Object);

	//�����ͼ¿��� ������ �������� ���� ��Ȯ�� ���ڿ� ������ �ʿ��� 
	mDataKey = TEXT("MinionRanged");

	mBody->SetCapsuleHalfHeight(110.5f);
	//�θ𿡼� ��������
	//mMesh->SetRelativeRotation(FRotator(0, -90, 0));
	mMesh->SetRelativeLocation(FVector(0, 0, -110.5));

	//�ִ��ν��Ͻ� (�������Ʈ ���� �Ҵ��ؾ� ��
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

//�������� ��� ������ ������ Ư�� �����ӿ��� ȣ���ϴ� ���� ������ �� �ִ�
void AMinionRanged::NormalAttack()
{
	AAIController* AI = GetController<AAIController>();

	if (IsValid(AI))
	{
		UObject* Obj = (AI->GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor")));

		AActor* Target = nullptr;

		if (Obj)
			Target = Cast<AActor>(Obj);
		
		//�߻� ����
		USoundBase* FireSFX = LoadObject<USoundBase>(GetWorld(), TEXT("/Script/Engine.SoundWave'/Game/Sounds/sfx/sfx_scifi_weapon_shoot_firing_pulse_dn_05.sfx_scifi_weapon_shoot_firing_pulse_dn_05'"));

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), FireSFX, mMesh->GetSocketLocation(TEXT("Muzzle_Front")));
		
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
