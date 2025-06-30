// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterTemplateAnimInstance.h"
#include "Monster.h"
#include "AIController.h"

UMonsterTemplateAnimInstance::UMonsterTemplateAnimInstance()
{
}

void UMonsterTemplateAnimInstance::AnimNotify_Attack()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->NormalAttack();
	}
}

void UMonsterTemplateAnimInstance::AnimNotify_AttackEnd()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		AAIController* AI = Monster->GetController<AAIController>();

		if (IsValid(AI))
		{
			AI->GetBlackboardComponent()->SetValueAsBool(TEXT("AttackEnd"), true);
		}
	}
}

void UMonsterTemplateAnimInstance::AnimNotify_Death()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->RagDoll();
		Monster->Destroy();
	}
}

void UMonsterTemplateAnimInstance::AnimNotify_Special()
{
	AMonster* Monster = Cast<AMonster>(TryGetPawnOwner());

	if (IsValid(Monster))
	{
		Monster->SpecialAttack();
	}
}

void UMonsterTemplateAnimInstance::AnimNotify_HitEnd()
{
	mHitAlpha = 0.f;
}
