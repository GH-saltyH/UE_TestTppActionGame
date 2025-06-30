// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestDataCacheManager.h"

UQuestDataCacheManager* UQuestDataCacheManager::mInst = nullptr;

UQuestDataCacheManager::UQuestDataCacheManager()
{
	//초기화 단계에서 데이터테이블을 읽어놓는다 -> 언제든 이 클래스르 통하여 꺼내올 수 있다.    
	//데이터 테이블 읽어놓기

	//static const FString MasterPath = TEXT("/Game/Data/Items/DataTables/DT_ItemMaster.DT_ItemMaster");

	//mItemMasterTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *MasterPath));

	static ConstructorHelpers::FObjectFinder<UDataTable>	
		QuestDT(TEXT("/Script/Engine.DataTable'/Game/Data/QuestSystem/DT_QuestData.DT_QuestData'"));

	if (QuestDT.Succeeded())
		mQuestDataTable = QuestDT.Object;
}


void UQuestDataCacheManager::Initialize(UDataTable* ItemMasterTable)
{

}


