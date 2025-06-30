// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"		//splinecomponent 사용을 위해 포함
#include "MonsterSpawnPoint.generated.h"

UCLASS()
class PRACTICEGAME_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnPoint();

protected:
	//가장 기본적인 루트컴포넌트를 들고 있을 포인터이다
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent>		mRoot;

	//스폰하려하는 클래스를 담을 포인터이다
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonster>		mSpawnClass;

	//이 멤버는 스폰시간을 지정한다
	UPROPERTY(EditAnywhere)
	float							mSpawnTime;

	//타이머 관련 기능을 조작할 핸들러이다
	FTimerHandle					mSpawnTimer;

	//스폰시 재생할 파티클시스템이다  !
	TObjectPtr<UParticleSystem>		mSpawnParticle;

	//실제 스폰한 몬스터
	class AMonster*					mSpawnMonster = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent>	mPatrolPath;

	//순찰돌 경로를 가지고 있느ㅜㄴ다 
	UPROPERTY(VisibleAnywhere)
	TArray<FVector>					mPatrolPoint;

	//에디터모드에 있을 경우 처리
#if WITH_EDITORONLY_DATA

	//에디터에서 방향을 볼 수 있도록 생성한 ㄴ컴포넌트이다.
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent>		mArrow;
#endif

public	:
	void ClearSpawnMonster()
	{
		mSpawnMonster = nullptr;

		if (mSpawnTime > 0.f)						//스폰시간이 0 보다 크면 타이머를 설정한다
		{
			//타이머 매니저 객체를 리턴받는다
			//타이머를 설정한다 
			GetWorldTimerManager().SetTimer(
				mSpawnTimer,							//타이머를 지정할 곳이다 
				this,									//타이머가 완료되었을 때 호출될 곳이다
				&AMonsterSpawnPoint::SpawnTimer,		//타이머 완료시 호출할 함수를 정한다
				mSpawnTime,								// 타이머의 길이를 정한다
				false);									//반복 실행 여부를 설정한다
		}

		else
			SpawnMonster();							//그렇지 않으면 몬스터를 스폰한다
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	void SpawnTimer();
	void SpawnMonster();
};
