// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include	"../Monster.h"
#include	"../../../Interface/CharacterPublicInterface.h"
#include "AIController.h"


UBTTask_Attack::UBTTask_Attack()
{
	NodeName = TEXT("MonsterAttack");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		Monster->ChangeAnim(EMonsterDefaultAnim::Attack);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		ICharacterPublicInterface* Info = Cast<ICharacterPublicInterface>(Monster);

		if (Info)
		{
			float	AttackDist = Info->GetAttackDistance();

			FVector	TargetLocation = FVector::ZeroVector, CurrentLocation;

			CurrentLocation = Monster->GetActorLocation();

			UCapsuleComponent* Capsule =
				Cast<UCapsuleComponent>(Monster->GetRootComponent());

			if (IsValid(Capsule))
				CurrentLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();

			UBlackboardComponent* Board = OwnerComp.GetAIOwner()->GetBlackboardComponent();

			bool	AttackEnd = false;

			if (IsValid(Board))
			{
				AttackEnd = Board->GetValueAsBool(TEXT("AttackEnd"));

				AActor* Target = Cast<AActor>(Board->GetValueAsObject(TEXT("TargetActor")));

				if (IsValid(Target))
				{
					TargetLocation = Target->GetActorLocation();

					Capsule =
						Cast<UCapsuleComponent>(Target->GetRootComponent());

					if (IsValid(Capsule))
						TargetLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
				}
			}

			if (AttackEnd)
			{
				Board->SetValueAsBool(TEXT("AttackEnd"), false);

				FRotator	Rot = GetTargetRotatorYaw(TargetLocation, CurrentLocation);

				Monster->SetActorRotation(Rot);

				// 두 위치 사이의 거리를 구한다.
				float	Dist = FVector::Dist(TargetLocation, CurrentLocation);

				if (Dist > AttackDist)
				{
					FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				}
			}
		}
	}
}

void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
	
}
