// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"		//splinecomponent ����� ���� ����
#include "MonsterSpawnPoint.generated.h"

UCLASS()
class PRACTICEGAME_API AMonsterSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawnPoint();

protected:
	//���� �⺻���� ��Ʈ������Ʈ�� ��� ���� �������̴�
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent>		mRoot;

	//�����Ϸ��ϴ� Ŭ������ ���� �������̴�
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AMonster>		mSpawnClass;

	//�� ����� �����ð��� �����Ѵ�
	UPROPERTY(EditAnywhere)
	float							mSpawnTime;

	//Ÿ�̸� ���� ����� ������ �ڵ鷯�̴�
	FTimerHandle					mSpawnTimer;

	//������ ����� ��ƼŬ�ý����̴�  !
	TObjectPtr<UParticleSystem>		mSpawnParticle;

	//���� ������ ����
	class AMonster*					mSpawnMonster = nullptr;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent>	mPatrolPath;

	//������ ��θ� ������ �ִ��̤��� 
	UPROPERTY(VisibleAnywhere)
	TArray<FVector>					mPatrolPoint;

	//�����͸�忡 ���� ��� ó��
#if WITH_EDITORONLY_DATA

	//�����Ϳ��� ������ �� �� �ֵ��� ������ ��������Ʈ�̴�.
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UArrowComponent>		mArrow;
#endif

public	:
	void ClearSpawnMonster()
	{
		mSpawnMonster = nullptr;

		if (mSpawnTime > 0.f)						//�����ð��� 0 ���� ũ�� Ÿ�̸Ӹ� �����Ѵ�
		{
			//Ÿ�̸� �Ŵ��� ��ü�� ���Ϲ޴´�
			//Ÿ�̸Ӹ� �����Ѵ� 
			GetWorldTimerManager().SetTimer(
				mSpawnTimer,							//Ÿ�̸Ӹ� ������ ���̴� 
				this,									//Ÿ�̸Ӱ� �Ϸ�Ǿ��� �� ȣ��� ���̴�
				&AMonsterSpawnPoint::SpawnTimer,		//Ÿ�̸� �Ϸ�� ȣ���� �Լ��� ���Ѵ�
				mSpawnTime,								// Ÿ�̸��� ���̸� ���Ѵ�
				false);									//�ݺ� ���� ���θ� �����Ѵ�
		}

		else
			SpawnMonster();							//�׷��� ������ ���͸� �����Ѵ�
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
