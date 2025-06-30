// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LookAround.h"
#include	"../Monster.h"
#include	"../../../Interface/CharacterPublicInterface.h"
#include "AIController.h"


UBTTask_LookAround::UBTTask_LookAround()
{
	NodeName = TEXT("LookAround");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_LookAround::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		Monster->ChangeAnim(EMonsterDefaultAnim::Idle);

		//�����带 ���´�
		UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

		//��ٸ��� �����ؾ� �ϱ� ������ ���� �ð��� 0���� �ʱ�ȭ�Ѵ�.
		if (IsValid(Board))
			Board->SetValueAsFloat(TEXT("AccLookAroundTime"), 0.f);

	}

	return EBTNodeResult::InProgress;
}

void UBTTask_LookAround::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

	if (IsValid(Board))
	{
		//�����忡�� ���� �ð��� �޾ƿ´�
		float AccTime = Board->GetValueAsFloat(TEXT("AccLookAroundTime"));

		//���� �ð��� ��Ÿ�����带 �����Ѵ�
		AccTime += DeltaSeconds;

		//�����ð� ���� ������ ����ð��� �� ������� �����ϰ� ����ó���Ѵ�
		if (AccTime >= Board->GetValueAsFloat(BlackboardKey.SelectedKeyName))
		{
			AccTime = 0.f;

			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}

		//AccTime ���� �����Ѵ� -> �̷��� �ϸ� �� ���͸��� �ٸ� �ڱ� �ð����� �����̵ȴ�
		Board->SetValueAsFloat(TEXT("AccLookAroundTime"), AccTime);
	}
}

void UBTTask_LookAround::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

}



