// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"
#include "../Data/Quest/QuestDataCacheManager.h"

// Sets default values for this component's properties
UQuestComponent::UQuestComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UQuestComponent::BeginPlay()
{
	Super::BeginPlay();

	// 태스크 타입별 퀘스트 핸들러 정의
	mTaskHandlers.Add(EQuestTaskType::KILL, [this](AActor* Target)
		{
			for (auto& Quest : mAcceptedQuests)
			{
				if (Quest->mTaskType != EQuestTaskType::KILL)
					continue;

				if (Target->IsA(Quest->mTargetNPCClass))
				{
					Quest->AddCount(Quest);
					if(CheckQuestCompletion(Quest))
						UE_LOG(TestGame, Warning, TEXT("Complete Objective : %s"), *Quest->mQuestID.ToString());
				}
			}
		});
	
	mTaskHandlers.Add(EQuestTaskType::TALK, [this](AActor* Target)
		{
			for (auto& Quest : mAcceptedQuests)
			{
				if (Quest->mTaskType != EQuestTaskType::TALK)
					continue;

				if (Target->IsA(Quest->mTargetNPCClass))
				{
					Quest->AddCount(Quest);
					if (CheckQuestCompletion(Quest))
						UE_LOG(TestGame, Warning, TEXT("Complete Objective : %s"), *Quest->mQuestID.ToString());
				}
			}
		});

	mTaskHandlers.Add(EQuestTaskType::ARRIVE, [this](AActor* Target)
		{
			for (auto& Quest : mAcceptedQuests)
			{
				if (Quest->mTaskType != EQuestTaskType::ARRIVE)
					continue;

				if (Target->IsA(Quest->mTargetOverlapVolumeClass))
				{
					Quest->AddCount(Quest);
					if (CheckQuestCompletion(Quest))
						UE_LOG(TestGame, Warning, TEXT("Complete Objective : %s"), *Quest->mQuestID.ToString());
				}
			}
		});

	mTaskHandlers.Add(EQuestTaskType::LOOT, [this](AActor* Target)
		{
			for (auto& Quest : mAcceptedQuests)
			{
				if (Quest->mTaskType != EQuestTaskType::LOOT)
					continue;

				if (Target->GetFName() == Quest->mTargetPickupActorClass->GetFName())
				{
					Quest->AddCount(Quest);
					if (CheckQuestCompletion(Quest))
						UE_LOG(TestGame, Warning, TEXT("Complete Objective : %s"), *Quest->mQuestID.ToString());
				}
			}
		});

	//TaskHandler 를 델리게이트 바인딩
	mOnQuestTriggered.AddUObject(this, &UQuestComponent::HandleQuestTrigger);
}

// Called every frame
void UQuestComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestComponent::AcceptQuest(UQuestData* Obj)
{
	if (!Obj)
		return;

	bool bQuestFound = false;

	//건네받은 퀘스트가 이미 리스트에 있는지 검사한다
	for (int32 i = 0; i < mAcceptedQuests.Num(); ++i)
	{
		if (Obj == mAcceptedQuests[i])
		{
			bQuestFound = true;
			break;
		}
	}

	//새로운 퀘스트라면 목록에 추가한다 
	if (!bQuestFound)
	{
		mAcceptedQuests.Add(Obj);
		++mAcceptedQuestCount;
		UE_LOG(TestGame, Warning, TEXT("QuestComponent : [%s] Quest Added to list"), *Obj->mQuestID.ToString());
	}
	else
		UE_LOG(TestGame, Warning, TEXT("QuestComponent : [%s] ALEADY  ADDED"), *Obj->mQuestID.ToString());
}

void UQuestComponent::RemoveQuest(UQuestData* Obj)
{
	if (!Obj)
		return;

	int32 removed = mAcceptedQuests.Remove(Obj);
	UE_LOG(TestGame, Warning, TEXT("tried Remove Quest [%s] :: Removed %d Quest(s) from player's list"), *Obj->mQuestID.ToString(), removed);
}

void UQuestComponent::HandleQuestTrigger(EQuestTaskType TaskType, AActor* Target)
{
	if (mTaskHandlers.Contains(TaskType))
	{
		//핸들러 실행
		mTaskHandlers[TaskType](Target);	
	}
}

bool UQuestComponent::CheckQuestCompletion(UQuestData* Obj)
{
	if (!Obj)
		return false;
	
	//퀘스트데이터 CDO 가져오기
	const UQuestDataCacheManager* CDO = GetDefault<UQuestDataCacheManager>();
	
	if (!CDO)
		return false;
	
	const FQuestInfo* GetRow = CDO->GetRowByQuestID(Obj->mQuestID);
	
	if (!GetRow)
		return false;
	
	//진척도가 요구사항을 초과해서 카운트되지 않도록 처리함
	int32 Current = Obj->mQuestProgressCount;
	int32 RequiredCount = GetRow->RequiredCount;
	
	UE_LOG(TestGame, Warning, TEXT("Quest Progress %d / %d"), Obj->mQuestProgressCount, GetRow->RequiredCount);

	//퀘스트 요구수치에 도달했는지 체크하여 true 반환, 아니면 false.  
	if (Current >= RequiredCount)
		return true;

	return false;
}
