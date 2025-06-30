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

		//블랙보드를 얻어온다
		UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

		//기다리기 시작해야 하기 떄문에 누적 시간을 0으로 초기화한다.
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
		//블랙보드에서 누적 시간을 받아온다
		float AccTime = Board->GetValueAsFloat(TEXT("AccLookAroundTime"));

		//누적 시간에 델타세컨드를 가산한다
		AccTime += DeltaSeconds;

		//누적시간 보다 보드의 경과시간이 더 길어지면 리셋하고 실패처리한다
		if (AccTime >= Board->GetValueAsFloat(BlackboardKey.SelectedKeyName))
		{
			AccTime = 0.f;

			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		}

		//AccTime 으로 설정한다 -> 이렇게 하면 각 엑터마다 다른 자기 시간으로 설정이된다
		Board->SetValueAsFloat(TEXT("AccLookAroundTime"), AccTime);
	}
}

void UBTTask_LookAround::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

}



