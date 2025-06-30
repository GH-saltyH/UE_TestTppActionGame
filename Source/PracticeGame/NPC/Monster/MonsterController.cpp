// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterController.h"
#include "Monster.h"

AMonsterController::AMonsterController()
{
	PrimaryActorTick.bCanEverTick = true;

	mAIPerception =
		CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));

	//퍼셉션 컴포넌트를 설정한다 
	SetPerceptionComponent(*mAIPerception);

	mSightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SenseSight"));

	//시야의 반경을 설정한다
	mSightConfig->SightRadius = 1000.f;
	//시야에서 벗어나는 판단지점을 설정한다
	mSightConfig->LoseSightRadius = 1000.f;
	//시야각을 설정한다
	//이 각도는 전방방향벡터 기준 단측면의 각도 값이다 
	//즉 양측면을 모두 합한각도가 전체 시야각이 된다 ( 두 배)
	mSightConfig->PeripheralVisionAngleDegrees = 50.f;

	//아군감지여부를 설정한다
	mSightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	//적군감지여부를 설정한다
	mSightConfig->DetectionByAffiliation.bDetectEnemies = true;
	//중립감지여부를 설정한다
	mSightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	//시야를 AI Perception 에 추가한다
	mAIPerception->ConfigureSense(*mSightConfig);

	//주 감각기관을 시야로 설정한다
	mAIPerception->SetDominantSense(mSightConfig->GetSenseImplementation());

	SetGenericTeamId(FGenericTeamId(TeamMonster));


	//Behavior Tree 를 로드하고 성공하면 할당한다
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
		// 시야 최대 거리
		mSightConfig->SightRadius = Monster->GetDetectDistance();

		// 감지된 대상이 시야 밖으로 사라졌다고 판단하는 거리
		mSightConfig->LoseSightRadius = Monster->GetDetectDistance();
		
		//센스를 설정함
		mAIPerception->ConfigureSense(*mSightConfig);
	}
			
	if (IsValid(mBehaviorTree))
		RunBehaviorTree(mBehaviorTree);

	// AIPerception이 타겟을 인지할 경우 호출될 함수를 등록한다.
	// 타겟이 빠져나갈 경우에도 호출된다.
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
	// 타겟이 감지된 경우
	if (Stimulus.WasSuccessfullySensed())
	{
		// 블랙보드에 감지된 타겟을 넣어준다.
		Blackboard->SetValueAsObject(TEXT("TargetActor"), Actor);
	}

	else
	{
		Blackboard->SetValueAsObject(TEXT("TargetActor"), nullptr);
	}
}
