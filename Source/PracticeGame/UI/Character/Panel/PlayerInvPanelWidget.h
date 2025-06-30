// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../UserWidgetBase.h"
#include "PlayerInvPanelWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UPlayerInvPanelWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	

public:
	UPlayerInvPanelWidget(const FObjectInitializer& ObjectInitializer);
	
protected:
	TObjectPtr<UListView>		mInventory;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D>		mItemIcon;
	TObjectPtr<UBorder>			mTitleBarBorder;
	TObjectPtr<UButton>			mButtonExit;

protected:
	virtual void NativeOnInitialized();

public:
	UFUNCTION()
	void ButtonCloseOnClick();

	//이동시켜주는 기능 함수
	UFUNCTION()
	FEventReply TitleMouseDown(FGeometry MyGeoMetry, const FPointerEvent& MouseEvent);
	UFUNCTION()
	FEventReply TitleMouseUp(FGeometry MyGeoMetry, const FPointerEvent& MouseEvent);
	UFUNCTION()
	FEventReply TitleMouseMove(FGeometry MyGeoMetry, const FPointerEvent& MouseEvent);
	
	UFUNCTION()
	void OnItemClick(UObject* ClickedItem);
	UFUNCTION()
	void OnItemDoubleClick(UObject* ClickedItem);
	UFUNCTION()
	void OnItemHovered(UObject* ClickedItem, bool IsHovered);
	UFUNCTION()
	void OnItemSelected(UObject* ClickedItem);
};
