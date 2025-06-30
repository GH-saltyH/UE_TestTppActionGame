// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UIInfo.h"
#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "SkillEntryList.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API USkillEntryList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USkillEntryList(const FObjectInitializer& ObjectInitializer);

protected:
	TObjectPtr<UImage>		mSkillIcon;
	TObjectPtr<UTextBlock>	mSkillName;
	TObjectPtr<UTextBlock>	mSkillInfo;

private:
	//데이터테이블로부터 현재 클래스 정보를 받아올것이다
	//필터된 데이터는 일반적으로 읽기 전용이므로 const 로 선언하는게 좋을 것 같다 -> 체크해보기?
	UPROPERTY()
	TArray<FPlayerSkillData>	mFilteredSkillData;

protected:
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();

public:
	//아이템 데이터 가져오기 
	UFUNCTION(BlueprintCallable)
	void SetSkillData(UObject* Data);
	
	
};
