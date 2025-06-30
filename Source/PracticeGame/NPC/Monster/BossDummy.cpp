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

	//�����ͼ¿��� ������ �������� ���� ��Ȯ�� ���ڿ� ������ �ʿ��� 
	mDataKey = TEXT("BossDummy01");

	mBody->SetCapsuleHalfHeight(200.f);
	//�θ𿡼� ��������
	//mMesh->SetRelativeRotation(FRotator(0, -90, 0));
	mMesh->SetRelativeLocation(FVector(0, 0, -200));
	mBody->SetCapsuleRadius(134.0f);

	//�ִ��ν��Ͻ� (�������Ʈ ���� �Ҵ��ؾ� ��
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

//�������� ��� ������ ������ Ư�� �����ӿ��� ȣ���ϴ� ���� ������ �� �ִ�
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

			//�������� ������
			Target->TakeDamage(mAttack, DmgEvent, AI, this);
		}
	}
}

//�������� ������ ������ ��Ÿ� �ȿ� �������� ȣ���ϴ��� 
// �������� ���������� ������ �˻��ؾ� �Ѵ�
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

			//�������� ������ �⺻ ������ 5 ��� �����Ѵ�
			Target->TakeDamage(mAttack * 5.0f, DmgEvent, AI, this);
			
			APlayerCharacter* PC = Cast<APlayerCharacter>(Target);
			//Ÿ���� �÷��̾� ĳ���� Ŭ������� VFX �� ������
			if(IsValid(PC))
			{
			//����� ��  "Chest"
				FVector	VFXLocation = PC->GetMesh()->GetSocketLocation(TEXT("Chest"));

				//if (!IsValid(GetWorld()))
				//{
				//	// �����ϰ� ���� �Ǵ� ��ü ó��
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

