// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "AIController.h"
#include "MonsterController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API AMonsterController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMonsterController();
	
protected:
	//�ֺ� ���� AI
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIPerceptionComponent> mAIPerception;

	//�ð� �ν� AI 
	TObjectPtr<UAISenseConfig_Sight>	mSightConfig;
	//�ǻ����Ʈ�� ������Ʈ
	TObjectPtr<UBehaviorTree>	mBehaviorTree;

public:
	virtual void OnConstruction(const FTransform& Transform);

protected:
	virtual void BeginPlay() override;

public:
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;

public:
	UFUNCTION()
	void OnTarget(AActor* Actor, FAIStimulus Stimulus);
};
