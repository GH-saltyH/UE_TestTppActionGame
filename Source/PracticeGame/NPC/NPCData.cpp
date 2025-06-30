// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCData.h"


UNPCData::UNPCData()
{

	static ConstructorHelpers::FObjectFinder<UDataTable>
		DataAsset(TEXT(
			"/Script/Engine.DataTable'/Game/NPC/DT_NPCInfo.DT_NPCInfo'"
		));

	if (DataAsset.Succeeded())
		mTable = DataAsset.Object;

}

//���� ���̺��� ���̸��� �������� �൥���͸� �о�´�
FNPCTableInfo* UNPCData::FindNPCInfo(
	const FName& Name)	const
{
	return mTable->FindRow<FNPCTableInfo>(Name, TEXT(""));
}


