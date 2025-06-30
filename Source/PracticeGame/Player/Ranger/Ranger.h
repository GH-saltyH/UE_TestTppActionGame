// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../PlayerCharacter.h"
#include "Ranger.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API ARanger : public APlayerCharacter
{
	GENERATED_BODY()
	
public:
	ARanger();

protected:
	FTimerHandle	mCleaveReleaseTimer;
	TObjectPtr<UNiagaraSystem>		mNiagaraCleaveHit;
	TObjectPtr<UNiagaraSystem>		mSkill1ProjectileAsset;
	//mSkillTimer1 �� ���콺 �����Ϳ� �������� ��Į�� �÷��׸� Ȱ��ȭ�ϴ� Ÿ�̸��̴�
	FTimerHandle	mSkillTimer1;
	FTimerHandle	mSkillTimer1Loop;
	//Skill1 �� ��Į �ּ�
	UDecalComponent* mSkill1Decal = nullptr;
	TObjectPtr<UMaterialInterface>	mSkill1DecalAsset;

protected:
	//virtual void RunSweepConeAndDamage(float Damage, float SphereSize, float AddedDistance, float InnerAngle, ECollisionChannel TraceChannel, UNiagaraSystem* VFXAsset = nullptr, bool bSpawnVFXOnHit = true, USoundBase* SFXAsset = nullptr, bool bSpawnSFXOnHit = true);
	////���� ���� �ٸ� �����ε� �Լ�
	//virtual void RunSweepConeAndDamage(float Damage, float SphereSize, float AddedDistance, float InnerAngle, ECollisionChannel TraceChannel, UParticleSystem* VFXAsset = nullptr, bool bSpawnVFXOnHit = true, USoundBase* SFXAsset = nullptr, bool bSpawnSFXOnHit = true);
public:
	void SpawnSkill1DecalAtLocation(FVector Location, FVector DecalSize)
	{
		if (!mSkill1Decal) // ������ ��Į�� ���� ��� ����
		{
			//UMaterialInterface* DecalAsset = LoadObject<UMaterialInterface>(nullptr,
			//	TEXT("/Game/ParagonMinions/FX/Textures/Runes/T_Decal_Astro.T_Decal_Astro"));

			if (mSkill1DecalAsset)
			{
				mSkill1Decal = UGameplayStatics::SpawnDecalAtLocation(
					GetWorld(),
					mSkill1DecalAsset,
					DecalSize, // ũ�� ���� ����
					Location,
					FRotator(90, 0, 0)); // ���� ����

				if (mSkill1Decal)
				{
					mSkill1Decal->SetFadeScreenSize(0.1f); // ���� �Ÿ����� ���̵� �ƿ�
				}
			}
		}
		else
		{
			//�̹� �����ϸ� ��ġ�� ������Ʈ
			mSkill1Decal->SetWorldLocation(Location);

			//UE_LOG(TestGame, Display, TEXT("Skill 1 : Decal location update"));
		}
	}

	void DeleteSkill1Decal()
	{
		//������ ��������, ������ �ʱ�ȭ
		if (mSkill1Decal)
		{
			mSkill1Decal->DestroyComponent();
			mSkill1Decal = nullptr;
		}
	}

public:
	virtual void NormalAttack() override;
protected:
	virtual void SpecialAttack() override;
	virtual void CleaveAttack() override;
	void ReleaseCleave();

	virtual void MakePreviewCastZone() override;
	virtual void CancelPreviewCastZone() override;
	virtual void AcceptPreviewCastZone() override;
	void UpdateSkill1Selection();
	void Skill1Init();
	void Skill1Loop(FVector TargetLocation, float SphereSize, float SpawnHeight);
public:
	virtual void PostInitializeComponents() override;
};
