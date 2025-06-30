// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Trace.h"
#include "../Monster.h"
#include "../../../Interface/CharacterPublicInterface.h"
#include "AIController.h"
#include "../../AIMovement.h"

UBTTask_Trace::UBTTask_Trace()
{
	NodeName = TEXT("MonsterTrace");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}


EBTNodeResult::Type UBTTask_Trace::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// 인자로 들어온 몬스터의 OwnerComponent를 이용해서 Monster를 얻어온다.
	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		UAIMovement* Movement = Monster->GetMonsterMovement();

		if (IsValid(Movement))
		{
			// 블랙보드 컴포넌트를 얻어온다.
			UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

			if (IsValid(Board))
			{
				UObject* Obj =
					Board->GetValueAsObject(TEXT("TargetActor"));

				AActor* Target = nullptr;

				if (Obj)
					Target = Cast<AActor>(Obj);

				if (IsValid(Target))
				{
					//OwnerComp.GetAIOwner()->MoveToActor(Target);
					Movement->SetTarget(Target);
				}
			}
		}
	}

	else
		return EBTNodeResult::Failed;

	// InProgress로 리턴을 하면 bNotifyTick = true; 를 해주었기 때문에
	//	 TickTask 가 계속해서 동작하게 된다.
	return EBTNodeResult::InProgress;
}

void UBTTask_Trace::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//공격거리 안에 들어갔을 경우 Task 를 종료해야 한다
	// 타겟과 AI 사이의 거리를 구하고 공격거리보다 작거나 같을 경우를 체크한다.
	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		ICharacterPublicInterface* Info = Cast<ICharacterPublicInterface>(Monster);

		if (Info)
		{
			float	AttackDist = Info->GetAttackDistance();

			FVector	TargetLocation, CurrentLocation;

			CurrentLocation = Monster->GetActorLocation();

			UCapsuleComponent* Capsule =
				Cast<UCapsuleComponent>(Monster->GetRootComponent());

			if (IsValid(Capsule))
				CurrentLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();

			UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

			if (IsValid(Board))
			{
				UObject* Obj =
					Board->GetValueAsObject(TEXT("TargetActor"));

				AActor* Target = nullptr;

				if (Obj)
					Target = Cast<AActor>(Obj);

				if (IsValid(Target))
				{
					TargetLocation = Target->GetActorLocation();

					Capsule =
						Cast<UCapsuleComponent>(Target->GetRootComponent());

					if (IsValid(Capsule))
						TargetLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
				}
			}

			// 두 위치 사이의 거리를 구한다.
			float	Dist = FVector::Dist(TargetLocation, CurrentLocation);

			//UE_LOG(TestGame, Warning, TEXT("Dist : %.5f, AttackDist : %.5f"),	Dist, AttackDist);

			if (Dist <= AttackDist)
			{
				FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
			}
		}
	}
}

void UBTTask_Trace::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);


	// 이동을 멈춘다.
	OwnerComp.GetAIOwner()->StopMovement();

	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		UAIMovement* Movement = Monster->GetMonsterMovement();

		if (IsValid(Movement))
		{
			Movement->ClearPath();
		}
	}
}
