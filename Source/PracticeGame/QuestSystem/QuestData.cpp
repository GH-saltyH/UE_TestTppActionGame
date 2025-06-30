
// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestData.h"
#include "../UI/WidgetManager.h"
#include "../QuestSystem/QuestWidget.h"
#include "../Data/Quest/QuestDataCacheManager.h"

bool UQuestData::AddCount(UObject* Obj, int32 AddNumber)
{
	if(!Obj)
		return false;

	UQuestData* IsQuest = Cast<UQuestData>(Obj);

	if (IsQuest)
	{
		//이 오브젝트의 QuestID 를 바탕으로 원본데이터에서 완료 카운트 찾기
		const UQuestDataCacheManager* CDO = GetDefault<UQuestDataCacheManager>();
		const FQuestInfo* RawData = CDO->GetRowByQuestID(IsQuest->mQuestID);

		//성공하면 카운트 계산 하기
		if (RawData)
		{
			int32 Goal = RawData->RequiredCount;

			mQuestProgressCount += AddNumber;

			//만약 퀘스트카운트가 목표에 도달했다면 퀘스트 플래그를 COMPLETED 로 설정한다.
			if (Goal <= mQuestProgressCount)
			{
				mThisProgress = EQuestProgressState::COMPLETED;		//이 퀘스트를 완료로 설정한다
				mQuestProgressCount = Goal;							//초과되어 카운트되지 않도록 한다

				return true;
			}
		}
	}
	return false;
}
