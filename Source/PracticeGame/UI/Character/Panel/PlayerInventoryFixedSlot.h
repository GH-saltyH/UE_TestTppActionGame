// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../UIInfo.h"
#include "../../UserWidgetBase.h"
#include "PlayerInventoryFixedSlot.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UPlayerInventoryFixedSlot : public UUserWidgetBase
{
	GENERATED_BODY()
	

public:
	UPlayerInventoryFixedSlot(const FObjectInitializer& ObjectInitializer);

protected:
	//각 슬롯
	TArray<TObjectPtr<class UPlayerInventorySlot>>	mInvSlots;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBorder>			mTitleBarBorder;
	TObjectPtr<UButton>			mButtonDragBar;
	TObjectPtr<UButton>			mButtonExit;
	TObjectPtr<UTextBlock>		mtextGold;
	class UInventoryComponent* mInventoryComponent = nullptr;

	bool	mTitleBarPressed = false;

protected:
	virtual void NativeOnInitialized();

public:
	UFUNCTION()
	void ButtonCloseOnClick();

public:
	void InitInventory(class UInventoryComponent* Inven);

public:
	void ChangeItem(class UInvItemData* Item, int32 Index);
	void SetGold(int32 Gold)
	{
		//변동된 골드를 현재 위젯에 반영해준다 -> 아직 위젯요소 안만들어서 잠시 주석처리
		FString GoldText = FString::FromInt(Gold) + TEXT(" G");
		//mTextGold->SetText(FText::FromString(GoldText));
	}
	//이동시켜주는 기능 함수
	UFUNCTION()
	void TitleMouseDown();
	UFUNCTION()
	void TitleMouseUp();
	//UFUNCTION()
	//FEventReply TitleMouseMove(FGeometry MyGeoMetry, const FPointerEvent& MouseEvent);

	//UFUNCTION()
	//void OnItemClick(UObject* ClickedItem);
	//UFUNCTION()
	//void OnItemDoubleClick(UObject* ClickedItem);
	//UFUNCTION()
	//void OnItemHovered(UObject* ClickedItem, bool IsHovered);
	//UFUNCTION()
	//void OnItemSelected(UObject* ClickedItem);
};
