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
	//mSkillTimer1 은 마우스 포인터에 지속적인 데칼과 플래그를 활성화하는 타이머이다
	FTimerHandle	mSkillTimer1;
	FTimerHandle	mSkillTimer1Loop;
	//Skill1 의 데칼 주소
	UDecalComponent* mSkill1Decal = nullptr;
	TObjectPtr<UMaterialInterface>	mSkill1DecalAsset;

protected:
	//virtual void RunSweepConeAndDamage(float Damage, float SphereSize, float AddedDistance, float InnerAngle, ECollisionChannel TraceChannel, UNiagaraSystem* VFXAsset = nullptr, bool bSpawnVFXOnHit = true, USoundBase* SFXAsset = nullptr, bool bSpawnSFXOnHit = true);
	////사운드 에셋 다른 오버로딩 함수
	//virtual void RunSweepConeAndDamage(float Damage, float SphereSize, float AddedDistance, float InnerAngle, ECollisionChannel TraceChannel, UParticleSystem* VFXAsset = nullptr, bool bSpawnVFXOnHit = true, USoundBase* SFXAsset = nullptr, bool bSpawnSFXOnHit = true);
public:
	void SpawnSkill1DecalAtLocation(FVector Location, FVector DecalSize)
	{
		if (!mSkill1Decal) // 스폰한 데칼이 없을 경우 생성
		{
			//UMaterialInterface* DecalAsset = LoadObject<UMaterialInterface>(nullptr,
			//	TEXT("/Game/ParagonMinions/FX/Textures/Runes/T_Decal_Astro.T_Decal_Astro"));

			if (mSkill1DecalAsset)
			{
				mSkill1Decal = UGameplayStatics::SpawnDecalAtLocation(
					GetWorld(),
					mSkill1DecalAsset,
					DecalSize, // 크기 조정 가능
					Location,
					FRotator(90, 0, 0)); // 영구 지속

				if (mSkill1Decal)
				{
					mSkill1Decal->SetFadeScreenSize(0.1f); // 일정 거리에서 페이드 아웃
				}
			}
		}
		else
		{
			//이미 존재하면 위치만 업데이트
			mSkill1Decal->SetWorldLocation(Location);

			//UE_LOG(TestGame, Display, TEXT("Skill 1 : Decal location update"));
		}
	}

	void DeleteSkill1Decal()
	{
		//있으면 스폰해제, 포인터 초기화
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
