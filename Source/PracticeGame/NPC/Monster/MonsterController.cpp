// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController.h"
#include "Monster.h"

AMonsterController::AMonsterController()
{
	PrimaryActorTick.bCanEverTick = true;

	mAIPerception =
		CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	//�ۼ��� ������Ʈ�� �����Ѵ� 
	SetPerceptionComponent(*mAIPerception);

	mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseSight"));

	//�þ��� �ݰ��� �����Ѵ�
	mSightConfig->SightRadius = 1000.f;
	//�þ߿��� ����� �Ǵ������� �����Ѵ�
	mSightConfig->LoseSightRadius = 1000.f;
	//�þ߰��� �����Ѵ�
	//�� ������ ������⺤�� ���� �������� ���� ���̴� 
	//�� �������� ��� ���Ѱ����� ��ü �þ߰��� �ȴ� ( �� ��)
	mSightConfig->PeripheralVisionAngleDegrees = 50.f;

	//�Ʊ��������θ� �����Ѵ�
	mSightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	//�����������θ� �����Ѵ�
	mSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	//�߸��������θ� �����Ѵ�
	mSightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	//�þ߸� AI Perception �� �߰��Ѵ�
	mAIPerception->ConfigureSense(*mSightConfig);

	//�� ��������� �þ߷� �����Ѵ�
	mAIPerception->SetDominantSense(mSightConfig->GetSenseImplementation());

	SetGenericTeamId(FGenericTeamId(TeamMonster));


	//Behavior Tree �� �ε��ϰ� �����ϸ� �Ҵ��Ѵ�
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>
		BehaviorTreeAsset(TEXT(
			"/Script/AIModule.BehaviorTree'/Game/Monster/BTMonster.BTMonster'"
		));

	if (BehaviorTreeAsset.Succeeded())
		mBehaviorTree = BehaviorTreeAsset.Object;
}

void AMonsterController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

void AMonsterController::BeginPlay()
{
	Super::BeginPlay();

	AMonster* Monster = GetPawn<AMonster>();

	if (IsValid(Monster))
	{
		// �þ� �ִ� �Ÿ�
		mSightConfig->SightRadius = Monster->GetDetectDistance();

		// ������ ����� �þ� ������ ������ٰ� �Ǵ��ϴ� �Ÿ�
		mSightConfig->LoseSightRadius = Monster->GetDetectDistance();
		
		//������ ������
		mAIPerception->ConfigureSense(*mSightConfig);
	}
			
	if (IsValid(mBehaviorTree))
		RunBehaviorTree(mBehaviorTree);

	// AIPerception�� Ÿ���� ������ ��� ȣ��� �Լ��� ����Ѵ�.
	// Ÿ���� �������� ��쿡�� ȣ��ȴ�.
	mAIPerception->OnTargetPerceptionUpdated.AddDynamic(this,
		&AMonsterController::OnTarget);

	Blackboard->SetValueAsFloat(TEXT("LookAroundTime"), 3.f);
}

ETeamAttitude::Type AMonsterController::GetTeamAttitudeTowards(
	const AActor& Other) const
{
	const IGenericTeamAgentInterface* OtherTeamAgent =
		Cast<const IGenericTeamAgentInterface>(&Other);

	if (!OtherTeamAgent)
		return ETeamAttitude::Neutral;

	else if (OtherTeamAgent->GetGenericTeamId().GetId() == TeamNeutral)
		return ETeamAttitude::Neutral;

	return GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId() ?
		ETeamAttitude::Friendly : ETeamAttitude::Hostile;
}

void AMonsterController::OnTarget(AActor* Actor,
	FAIStimulus Stimulus)
{
	// Ÿ���� ������ ���
	if (Stimulus.WasSuccessfullySensed())
	{
		// �����忡 ������ Ÿ���� �־��ش�.
		Blackboard->SetValueAsObject(TEXT("TargetActor"), Actor);
	}

	else
	{
		Blackboard->SetValueAsObject(TEXT("TargetActor"), nullptr);
	}
}
