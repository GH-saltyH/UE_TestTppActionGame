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
	//���������̺�κ��� ���� Ŭ���� ������ �޾ƿð��̴�
	//���͵� �����ʹ� �Ϲ������� �б� �����̹Ƿ� const �� �����ϴ°� ���� �� ���� -> üũ�غ���?
	UPROPERTY()
	TArray<FPlayerSkillData>	mFilteredSkillData;

protected:
	virtual void NativeOnInitialized();
	virtual void NativeConstruct();

public:
	//������ ������ �������� 
	UFUNCTION(BlueprintCallable)
	void SetSkillData(UObject* Data);
	
	
};
