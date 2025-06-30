// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Patrol.h"
#include	"../Monster.h"
#include	"../../../Interface/CharacterPublicInterface.h"
#include "AIController.h"

UBTTask_Patrol::UBTTask_Patrol()
{
	//패트롤 노드로 설정한다
	NodeName = TEXT("Patrol");
	bNotifyTick = true;		//매 프레임 이벤트 호출 활성화
	bNotifyTaskFinished = true; //태스크완료시 이벤트 호출 활성화
}

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();
	
	//몬스터를 얻어오지 못할 시 결과를 Failed 로 해둔다
	if (!IsValid(Monster))
		return EBTNodeResult::Failed;

	//몬스터 애니메이션을 걷기로 변겯한다 
	Monster->ChangeAnim(EMonsterDefaultAnim::Walk);

	UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

	if (!IsValid(Board))
		return EBTNodeResult::Failed;

	//몬스터가 있고 보드를 얻어왔으면 몬스터의 패트롤 포인트를 받아온다
	FVector Point = Monster->GetPatrolPoint();

	//보드의 PatrolPoint 값을 받아온 몬스터의 패트롤 Point 로 한다
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

	//이동을 멈추게 한다
	OwnerComp.GetAIOwner()->StopMovement();

	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		//목적지에 도착해서 종료되었다
		if (TaskResult == EBTNodeResult::Succeeded)
			Monster->NextPatrolPoint();

		//애니메이션을 Idle 로 전환한다
		Monster->ChangeAnim(EMonsterDefaultAnim::Idle);
	}

}


