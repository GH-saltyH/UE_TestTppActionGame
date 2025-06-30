// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInvPanelWidget.h"
#include "../InvItemData.h"
#include "../../../Data/Item/ItemDataCacheManager.h"

UPlayerInvPanelWidget::UPlayerInvPanelWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	mWidgetName = TEXT("PlayerInvPanelWidget");
}

void UPlayerInvPanelWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mInventory = Cast<UListView>(GetWidgetFromName(TEXT("InventoryList")));
	mTitleBarBorder = Cast<UBorder>(GetWidgetFromName(TEXT("TitleBarBorder")));
	mButtonExit = Cast<UButton>(GetWidgetFromName(TEXT("ButtonExit")));

	//닫기 버튼 델리게이트 함수 바인딩
	mButtonExit->OnClicked.AddDynamic(this, &UPlayerInvPanelWidget::ButtonCloseOnClick);


	//제목바 마우스다운 / 마우스업 / 마우스무브 이벤트 함수 바인딩
	mTitleBarBorder->OnMouseButtonDownEvent.BindUFunction(this, TEXT("TitleMouseDown"));
	mTitleBarBorder->OnMouseButtonUpEvent.BindUFunction(this, TEXT("TitleMouseUp"));
	mTitleBarBorder->OnMouseMoveEvent.BindUFunction(this, TEXT("TitleMouseMove"));

	//개별 아이템의 클릭 / 더븤클릭 / 호버드 / 선택  이벤트 함수 바인딩
	mInventory->OnItemClicked().AddUObject(this, &UPlayerInvPanelWidget::OnItemClick);
	mInventory->OnItemDoubleClicked().AddUObject(this, &UPlayerInvPanelWidget::OnItemDoubleClick);
	mInventory->OnItemIsHoveredChanged().AddUObject(this, &UPlayerInvPanelWidget::OnItemHovered);
	mInventory->OnItemSelectionChanged().AddUObject(this, &UPlayerInvPanelWidget::OnItemSelected);
}

void UPlayerInvPanelWidget::ButtonCloseOnClick()
{
	if (IsVisible())
		SetVisibility(ESlateVisibility::Collapsed);

}

FEventReply UPlayerInvPanelWidget::TitleMouseDown(FGeometry MyGeoMetry, const FPointerEvent& MouseEvent)
{
	UE_LOG(TestGame, Warning, TEXT("TitleMouseDown"));
	return FEventReply();

	
}

FEventReply UPlayerInvPanelWidget::TitleMouseUp(FGeometry MyGeoMetry, const FPointerEvent& MouseEvent)
{
	UE_LOG(TestGame, Warning, TEXT("TitleMouseUp"));
	return FEventReply();
	
}

FEventReply UPlayerInvPanelWidget::TitleMouseMove(FGeometry MyGeoMetry, const FPointerEvent& MouseEvent)
{
	UE_LOG(TestGame, Warning, TEXT("TitleMouseMove"));
	return FEventReply();
	
}

void UPlayerInvPanelWidget::OnItemClick(UObject* ClickedItem)
{
	UE_LOG(TestGame, Warning, TEXT("Item Clicked"));
}

void UPlayerInvPanelWidget::OnItemDoubleClick(UObject* ClickedItem)
{
	UE_LOG(TestGame, Warning, TEXT("Item Doublie Clicked"));
	mInventory->ClearListItems();
}

void UPlayerInvPanelWidget::OnItemHovered(UObject* ClickedItem, bool IsHovered)
{
	UE_LOG(TestGame, Warning, TEXT("Item Hovered"));
}

void UPlayerInvPanelWidget::OnItemSelected(UObject* ClickedItem)
{
	UE_LOG(TestGame, Warning, TEXT("Item Selected"));
}
