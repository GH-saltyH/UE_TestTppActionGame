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

//���� ���̺��� ���̸��� �������� �൥���͸� �о�´�
FMonsterTableInfo* UMonsterData::FindMonsterInfo(
	const FName& Name)	const
{
	return mTable->FindRow<FMonsterTableInfo>(Name, TEXT(""));
}
