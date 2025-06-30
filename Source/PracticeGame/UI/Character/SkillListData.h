// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UIInfo.h"
#include "../../GameInfo.h"
#include "Panel/PlayerSkillPanelWidget.h"
#include "UObject/NoExportTypes.h"
#include "SkillListData.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API USkillListData : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TObjectPtr<UPlayerSkillPanelWidget> mOwnerPanel;

private:
	FPlayerSkillData mSkillData;
	
public:
	const FPlayerSkillData& GetSkillData() const
	{
		return mSkillData;
	}
	
	void SetSkillData(const FPlayerSkillData& NewData)
	{
		mSkillData = NewData;
	}
};
