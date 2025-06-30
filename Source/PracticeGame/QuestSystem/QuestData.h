// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "QuestData.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UQuestData : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName mQuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName mParentQuestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestLevel mQuestLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestTaskType  mTaskType;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FString mTitle;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FString mDescription;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FString mObjective;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> mTargetOverlapVolumeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn>	mTargetNPCClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor>	mTargetPickupActorClass;

	UPROPERTY()
	TArray<TObjectPtr<UQuestData>>	mChildArray;

	int32 mTreeLevel=0;
	bool mSelect = false;

	
	//여기서 부터는 진행정보를 체크하기 위한 멤버들이다
	//1 진행정보 플래그
	EQuestProgressState		mThisProgress = EQuestProgressState::NOTACCEPTED;
	//2. 진행정도 저장멤버
	int32					mQuestProgressCount=0;

public:
	//강사님 코드
	/*QuestData* CreateChild(const FString& QuestName)
	{
		UQuestData* Child = NewObject<UQuestData>();

		Child->mTitle = QuestName;
		Child->mTreeLevel = mTreeLevel + 1;

		mChildArray.Add(Child);

		return Child;
	}*/


	//내 코드: 자식 노드 더하기
	void AddChild(UQuestData* Child)
	{
		if (Child)
			mChildArray.Add(Child);
	}

	//퀘스트 프로그레션을 더한다, 만약 목표에 도달하면 True 를 반환한다.
	bool AddCount(UObject* Obj, int32 AddNumber = 1);

	//건네 받은 행위가 현재 퀘스트에 해당하는지 체크한다 맞다면 카운트를 호출한다 
};
