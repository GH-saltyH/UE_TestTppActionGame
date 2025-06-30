// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerData.h"




UPlayerData::UPlayerData()
{

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DataAsset(TEXT(
			"/Script/Engine.DataTable'/Game/Player/DTPlayerInfo.DTPlayerInfo'"
		));
	if (DataAsset.Succeeded())
		mTable = DataAsset.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> ExpAsset(TEXT("/Script/Engine.DataTable'/Game/Player/DTExpTable.DTExpTable'"));
	if (ExpAsset.Succeeded())
		mExpTable = ExpAsset.Object;

}

//플레이어인포 테이블에서 행이름을 기준으로 행데이터를 읽어온다
FPlayerTableInfo* UPlayerData::FindPlayerInfo(
	const FName& Name)	const
{
	return mTable->FindRow<FPlayerTableInfo>(Name, TEXT(""));
}

//Exp테이블에서 행데이터를 읽어온다
FLevelExpRow* UPlayerData::FindExpInfo(
	const FName& Name)	const
{
	return mExpTable->FindRow<FLevelExpRow>(Name, TEXT(""));
}