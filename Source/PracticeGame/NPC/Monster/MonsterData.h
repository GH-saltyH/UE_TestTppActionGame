// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "MonsterData.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UMonsterData : public UObject
{
	GENERATED_BODY()
	
public:
	UMonsterData();

	//테이블을 들고 있는다
	TObjectPtr<UDataTable>	mTable;
	
public:
	FMonsterTableInfo* FindMonsterInfo(const FName& Name)	const;
};
