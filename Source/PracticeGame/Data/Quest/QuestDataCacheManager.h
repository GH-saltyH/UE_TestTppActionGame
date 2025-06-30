// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "QuestDataCacheManager.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UQuestDataCacheManager : public UObject
{
	GENERATED_BODY()
	
private:
	UQuestDataCacheManager();

	//UObject 의 싱글턴 패턴 
private:
	static UQuestDataCacheManager* mInst;

public:
	//UObject 를 상속하고 있기 때문에 루트 추가 제거가 필요함.
	static UQuestDataCacheManager* GetInst()
	{
		if (!mInst)
		{
			mInst = NewObject<UQuestDataCacheManager>();
			mInst->AddToRoot(); // GC 방지
		}
		return mInst;
	}

	static void DestroyInst()
	{
		if (mInst)
		{
			mInst->RemoveFromRoot(); // GC에 다시 맡김
			mInst = nullptr;
		}
	}

private:
	UPROPERTY()
	UDataTable* mQuestDataTable;

public:
	void Initialize(UDataTable* DataTable);

public:
	UDataTable* GetQuestTable() const
	{
		return mQuestDataTable;
	}

	TArray<FName>	GetAllQuestRowNames() const
	{
		TArray<FName> RowNames;

		if (mQuestDataTable)
		{
			RowNames = mQuestDataTable->GetRowNames();
		}

		return RowNames;
	}

	const FQuestInfo* GetRowByQuestID(const FName& ToFindQuestID) const
	{
		if (!mQuestDataTable)
			return nullptr;

		//전달 받은 퀘스트 ID 를   데이터테이블의 QuestID 필드에서 검색해서 있으면 반환한다
		for (auto& Row : mQuestDataTable->GetRowMap())
		{
			FQuestInfo* QuestInfo = reinterpret_cast<FQuestInfo*>(Row.Value);
			if (QuestInfo && QuestInfo->QuestID == ToFindQuestID)
			{
				return QuestInfo;
			}
		}
		
		return nullptr;
	}
};
