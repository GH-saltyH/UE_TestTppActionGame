// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Special.generated.h"

/**
 * Special 능력에 대한 테스크 이다 
 * 주로 보스와 같은 몬스터 객체에서 활용된다
 */
UCLASS()
class PRACTICEGAME_API UBTTask_Special : public UBTTaskNode
{
	GENERATED_BODY()
	

public:
	UBTTask_Special();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);
};