// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../NPC.h"
#include "../AIMovement.h"
//#include "../NPCFloatingPawnMovement.h"
#include "AIController.h"
#include "../../Interface/CharacterPublicInterface.h"
#include "Monster.generated.h"

UCLASS()
class PRACTICEGAME_API AMonster
	: public ANPC,
	public ICharacterPublicInterface
{
	GENERATED_BODY()

public:
	AMonster();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIMovement>	mMovement;

	UPROPERTY(EditAnywhere)
	float			mDetectDistance;

	//weak_ptr 도 유용하다
	//TWeakPtr<>  XXXXX
	class AMonsterSpawnPoint* mSpawnPoint = nullptr;

	//몬스터의 애니메이션을 가지고 있을 애님인스턴스 포인터이다
	TObjectPtr<class UMonsterTemplateAnimInstance>	mMonsterAnim;

	//순찰돌 경로를 가지고 있느ㅜㄴ다 
	UPROPERTY(VisibleAnywhere)
	TArray<FVector>		mPatrolPoint;

	//패트롤 위치를 선언한다,  패트롤의 시작점은 1이므로 1으로 초기화한다
	int32				mPatrolIndex = 1;

	TArray<TObjectPtr<UMaterialInstanceDynamic>>		mMaterials;

	//true 가 되면 번효과를 적용하게끔 선언한다
	bool				mIsBurn = false;
	float				mBurn = 0.1f;

	//맞았을 때 번쩍하는 효과를 내기 위한 플래그이다 true 면 머티리얼 그래프에서 연결된 노드가 활성화되게 할 것이다
	bool				mHitFXEnable = false;
	float				mHitTime = 0.f;

	//한 번이라도 맞으면 true 가된다 -> 이걸 기준으로 생명력 플레이트를 머리에  띄울 것이다
	bool 				mIsHit = false;

public:
	//몬스터의 현재 패트롤포인트 FVector 를 받아온다 
	const FVector& GetPatrolPoint() const
	{
		return mPatrolPoint[mPatrolIndex];
	}

	//다음 계산에서의 패트롤포인트를 순환 증가시킨다
	void NextPatrolPoint()
	{
		//패트롤 포인트가 마지막에서 순환하게 한다
		mPatrolIndex = (mPatrolIndex + 1) % mPatrolPoint.Num();
	}

	UAIMovement* GetMonsterMovement()	const
	{
		return mMovement;
	}

	float GetDetectDistance() const
	{
		return mDetectDistance;
	}

	int32 GetExp() const
	{
		return mExp;
	}

	int32 GetGold() const
	{
		return	mGold;
	}

public:
	//다음 틱의 애니메이션을 변경한다
	void ChangeAnim(EMonsterDefaultAnim Anim);

	void AddPatrolPoint(const FVector& Point)
	{

		mPatrolPoint.Add(Point);

		if (mPatrolPoint.Num() == 1)
		{
			AAIController* AI = GetController<AAIController>();

			AI->GetBlackboardComponent()->SetValueAsBool(TEXT("PatrolEnable"), true);
		}
	}

	void SetSpawnPoint(AMonsterSpawnPoint* Point)
	{
		mSpawnPoint = Point;
	}

	////포인터 사용에 오류를 줄이기 위해 필요한 함수이다
	//void ClearSpawnMonster()
	//{
	//	mSpawnPoint = nullptr;

	//}

    void SetDetectDistance(float NewDistance)
    {
        mDetectDistance= NewDistance;
    }

	void AddExp(int32 Exp)
	{
		mExp += Exp;
	}

	void SetExp(int32 Exp)
	{
		mExp = Exp;
	}

	void AddGold(int32 Gold)
	{
		mGold += Gold;
	}

	void SetGold(int32 Gold)
	{
		mGold = Gold;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason)	override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void OnConstruction(const FTransform& Transform);

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

	virtual void NormalAttack();
	virtual void SpecialAttack();
	void RagDoll();

	virtual void UpdateHPBar();
};
