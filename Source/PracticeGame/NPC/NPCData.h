// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "NPCData.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UNPCData : public UObject
{
	GENERATED_BODY()

public:
	UNPCData();

	//���̺��� ��� �ִ´�
	TObjectPtr<UDataTable>	mTable;

public:
	FNPCTableInfo* FindNPCInfo(const FName& Name)	const;
	
	
	
};
