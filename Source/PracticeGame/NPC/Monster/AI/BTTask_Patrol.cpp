// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include	"../Monster.h"
#include	"../../../Interface/CharacterPublicInterface.h"
#include "AIController.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	//��Ʈ�� ���� �����Ѵ�
	NodeName = TEXT("Patrol");
	bNotifyTick = true;		//�� ������ �̺�Ʈ ȣ�� Ȱ��ȭ
	bNotifyTaskFinished = true; //�½�ũ�Ϸ�� �̺�Ʈ ȣ�� Ȱ��ȭ
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();
	
	//���͸� ������ ���� �� ����� Failed �� �صд�
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	//���� �ִϸ��̼��� �ȱ�� �����Ѵ� 
	Monster->ChangeAnim(EMonsterDefaultAnim::Walk);

	UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

	if (!IsValid(Board))
		return EBTNodeResult::Failed;

	//���Ͱ� �ְ� ���带 �������� ������ ��Ʈ�� ����Ʈ�� �޾ƿ´�
	FVector Point = Monster->GetPatrolPoint();

	//������ PatrolPoint ���� �޾ƿ� ������ ��Ʈ�� Point �� �Ѵ�
	Board->SetValueAsVector(TEXT("PatrolPoint"), Point);

	return Result;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTTask_Patrol::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	//�̵��� ���߰� �Ѵ�
	OwnerComp.GetAIOwner()->StopMovement();

	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		//�������� �����ؼ� ����Ǿ���
		if (TaskResult == EBTNodeResult::Succeeded)
			Monster->NextPatrolPoint();

		//�ִϸ��̼��� Idle �� ��ȯ�Ѵ�
		Monster->ChangeAnim(EMonsterDefaultAnim::Idle);
	}

}


