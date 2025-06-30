// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "../SkillListData.h"
#include "PlayerActionSlot.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UPlayerActionSlot : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPlayerActionSlot(const FObjectInitializer& ObjectInitializer);

protected:
	//�ڱ��ڽ��� �����ε����� ��� ���� �� �ְ� �Ѵ� -> �κ��丮������Ʈ �ʱ�ȭ �� �������̴�
	int32								mSlotIndex = 0;
	TObjectPtr<UImage>		mSlotIcon;
	TObjectPtr<UImage>		mSlotBorder;

	//�巡�� �����̴� ���÷����� �ʿ��ϴ�
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerActionSlot>		mDragWidgetClass;
	class USkillComponent* mSkillComponent = nullptr;

protected:
	virtual void NativeOnInitialized();
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);

public:
	void SetSlotIndex(int32 NewIndex)
	{
		mSlotIndex = NewIndex;
	}

	void ChangeSkill(USkillListData* Skill);
	//����ְ� ó�� 

	void SetSkillComponent(class USkillComponent* SkillComponent)
	{
		mSkillComponent = SkillComponent;
	}
};
