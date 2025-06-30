// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Special.generated.h"

/**
 * Special �ɷ¿� ���� �׽�ũ �̴� 
 * �ַ� ������ ���� ���� ��ü���� Ȱ��ȴ�
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