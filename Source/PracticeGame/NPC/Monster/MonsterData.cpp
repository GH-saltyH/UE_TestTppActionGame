// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterData.h"

UMonsterData::UMonsterData()
{

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DataAsset(TEXT(
			"/Script/Engine.DataTable'/Game/Monster/DTMonsterInfo.DTMonsterInfo'"
		));

	if (DataAsset.Succeeded())
		mTable = DataAsset.Object;

}

//몬스터 테이블에서 행이름을 기준으로 행데이터를 읽어온다
FMonsterTableInfo* UMonsterData::FindMonsterInfo(
	const FName& Name)	const
{
	return mTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}
