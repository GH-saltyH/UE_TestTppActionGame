// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "../InvItemData.h"
#include "PlayerInventorySlot.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UPlayerInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPlayerInventorySlot(const FObjectInitializer& ObjectInitializer);

protected:
	//자기자신의 슬롯인덱스를 들고 있을 수 있게 한다 -> 인벤토리컴포넌트 초기화 때 넣을것이다
	int32					mSlotIndex = 0;
	TObjectPtr<UImage>		mSlotIcon;
	TObjectPtr<UImage>		mSlotBorder;

	//드래그 위젯이다 리플렉션이 필요하다
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPlayerInventorySlot>	mDragWidgetClass;
	class UInventoryComponent* mInventoryComponent = nullptr;

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
	void ChangeItem(UInvItemData* Item);
	//들고있게 처리 
	void SetInventoryComponent(class UInventoryComponent* Inventory)
	{
		mInventoryComponent = Inventory;
	}
};
