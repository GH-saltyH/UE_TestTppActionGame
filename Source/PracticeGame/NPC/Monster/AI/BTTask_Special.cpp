// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Special.h"
#include	"../Monster.h"
#include	"../../../Interface/CharacterPublicInterface.h"
#include "AIController.h"


UBTTask_Special::UBTTask_Special()
{
	NodeName = TEXT("SpecialAttack");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_Special::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type	Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AMonster* Monster = OwnerComp.GetAIOwner()->GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		Monster->ChangeAnim(EMonsterDefaultAnim::Special);
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Special::TickTask(UBehaviorTreeComponent& OwnerComp,
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

			bool	CanSpecial = false;
			bool	IsHPHalved = false;


			if (IsValid(Board))
			{
				CanSpecial = Board->GetValueAsBool(TEXT("CanSpecial"));
				IsHPHalved = Board->GetValueAsBool(TEXT("IsHPHalved"));

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

			//���� �����̰� ������� ��� ������ ��  => 1ȸ ��� �� �����ǰ� �����غ���
			if (CanSpecial && IsHPHalved)
			{
				Board->SetValueAsBool(TEXT("CanSpecial"), false);

				FRotator	Rot = GetTargetRotatorYaw(TargetLocation, CurrentLocation);

				Monster->SetActorRotation(Rot);

				// ���Ǵ޼��� �Ÿ� ������� �����Ѵ� -> �ּ�ó��
				//// �� ��ġ ������ �Ÿ��� ���Ѵ�.
				//float	Dist = FVector::Dist(TargetLocation, CurrentLocation);

				//if (Dist > AttackDist)
				//{
				//	FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
				//}
			}
		}
	}
}

void UBTTask_Special::OnTaskFinished(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

}
