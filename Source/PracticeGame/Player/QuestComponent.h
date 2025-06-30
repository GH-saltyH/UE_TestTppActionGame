// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ActorComponent.h"
#include "../QuestSystem/QuestData.h"
#include "QuestComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuestTriggerEvent, EQuestTaskType, AActor*);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICEGAME_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestComponent();

public:
	//수락한스킬의 리스트를 담는다.
	//UQuestData 객체의 포인터를 참조해야 
	//해당 클래스 내부의 퀘스트 진척도와 관련한 플래그를 정확히 추적하고 관리할 수 있다
	TArray<TObjectPtr<UQuestData>>	mAcceptedQuests;
	//혹시 모르니 수락중인 퀘스트 카운트
	int32												mAcceptedQuestCount = 0;

	//라우터 (디스패처)
	TMap < EQuestTaskType, TFunction<void(AActor* Target)>> mTaskHandlers;
	
	//델리게이트 
	FOnQuestTriggerEvent mOnQuestTriggered;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void AcceptQuest(UQuestData* Obj);
	void RemoveQuest(UQuestData* Obj);

	//퀘스트 핸들러 -> 각 이벤트마다 이를 호출하면 mTaskHandler 에 정의된 동작을 수행할 것이다 -> 델리게이트 바인딩 대상이다
	UFUNCTION()
	void HandleQuestTrigger(EQuestTaskType TaskType, AActor* Target);

	//현재 퀘스트의 카운트와 데이터테이블상 요구카운트를 비교하여 달성시 true  아니면  false
	bool  CheckQuestCompletion(UQuestData* Obj);
};
