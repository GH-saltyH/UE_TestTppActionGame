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

//�÷��̾����� ���̺��� ���̸��� �������� �൥���͸� �о�´�
FPlayerTableInfo* UPlayerData::FindPlayerInfo(
	const FName& Name)	const
{
	return mTable->FindRow<FPlayerTableInfo>(Name, TEXT(""));
}

//Exp���̺��� �൥���͸� �о�´�
FLevelExpRow* UPlayerData::FindExpInfo(
	const FName& Name)	const
{
	return mExpTable->FindRow<FLevelExpRow>(Name, TEXT(""));
}