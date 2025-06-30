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

	// ���ڷ� ���� ������ OwnerComponent�� �̿��ؼ� Monster�� ���´�.
	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		UAIMovement* Movement = Monster->GetMonsterMovement();

		if (IsValid(Movement))
		{
			// ������ ������Ʈ�� ���´�.
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

	// InProgress�� ������ �ϸ� bNotifyTick = true; �� ���־��� ������
	//	 TickTask �� ����ؼ� �����ϰ� �ȴ�.
	return EBTNodeResult::InProgress;
}

void UBTTask_Trace::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//���ݰŸ� �ȿ� ���� ��� Task �� �����ؾ� �Ѵ�
	// Ÿ�ٰ� AI ������ �Ÿ��� ���ϰ� ���ݰŸ����� �۰ų� ���� ��츦 üũ�Ѵ�.
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

			// �� ��ġ ������ �Ÿ��� ���Ѵ�.
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


	// �̵��� �����.
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
