// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventoryFixedSlot.h"
#include "PlayerInventorySlot.h"
#include "../InvItemData.h"
#include "../../../Player/InventoryComponent.h"
#include "../../MainWidget.h"

UPlayerInventoryFixedSlot::UPlayerInventoryFixedSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	mWidgetName = TEXT("PlayerInventoryFixedSlot");
}

void UPlayerInventoryFixedSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//요소들 받아두기
	mTitleBarBorder = Cast<UBorder>(GetWidgetFromName(TEXT("TitleBarBorder")));
	mButtonDragBar = Cast<UButton>(GetWidgetFromName(TEXT("TitleBarButton")));
	mButtonExit = Cast<UButton>(GetWidgetFromName(TEXT("ButtonExit")));

	//닫기 버튼 델리게이트 함수 바인딩
	mButtonExit->OnClicked.AddDynamic(this, &UPlayerInventoryFixedSlot::ButtonCloseOnClick);

	mButtonDragBar->OnPressed.AddDynamic(this, &UPlayerInventoryFixedSlot::TitleMouseDown);
	mButtonDragBar->OnReleased.AddDynamic(this, &UPlayerInventoryFixedSlot::TitleMouseUp);
}

void UPlayerInventoryFixedSlot::ButtonCloseOnClick()
{
	//창닫기 버튼
	if (IsVisible())
		SetVisibility(ESlateVisibility::Collapsed);
}

void UPlayerInventoryFixedSlot::InitInventory(UInventoryComponent* Inv)
{
	mInventoryComponent = Inv;
	int32	Count = Inv->GetInvSlotLimit();

	for (int32 i = 0; i < Count; ++i)
	{
		FString	WidgetName = TEXT("InvSlot_") +
			FString::FromInt(i);

		UPlayerInventorySlot* InvSlot =
			Cast<UPlayerInventorySlot>(GetWidgetFromName(*WidgetName));

		//슬롯번호 설정 및 아이템 슬롯아이콘 초기화
		InvSlot->SetSlotIndex(i);
		InvSlot->ChangeItem(nullptr);
		//slot 에도 컴포넌트 주소를 전달해서 기억하게 한다(모든 곳에서 처리하기 쉽게
		InvSlot->SetInventoryComponent(Inv);
		mInvSlots.Add(InvSlot);
	}

	// 멀티캐스트 델리게이트 바인딩 시 
	Inv->mInvChangeDelegate.AddUObject(this, &UPlayerInventoryFixedSlot::ChangeItem);
	Inv->mChangeGoldDelegate.AddUObject(this, &UPlayerInventoryFixedSlot::SetGold);
}

void UPlayerInventoryFixedSlot::ChangeItem(UInvItemData* Item, int32 Index)
{
	//아이템 정보 반영하기 -> 개별 슬롯은 위젯이아닌 개별 슬롯위젯에서 처리해야 하므로 해당 위젯의 ChangeItem 으로 처리하도록 전달한다
	mInvSlots[Index]->ChangeItem(Item);
}


void UPlayerInventoryFixedSlot::TitleMouseDown()
{

	UE_LOG(TestGame, Warning, TEXT("Inventory Title Mouse Down"));

	//메인위젯에 드래그 위젯으로 이 위젯을 전달한다.  
	//메인위젯 단위에서 드래그 행위와 관련된 계산을 하는데 유용하게 될 것이다
	UMainWidget* MainWidget = Cast<UMainWidget>(mParentWidget);

	if (MainWidget)
	{
		MainWidget->SetDragWidget(this);
		MainWidget->SetFocusWidget(this);
	}
}

void UPlayerInventoryFixedSlot::TitleMouseUp()
{
	UE_LOG(TestGame, Warning, TEXT("Inventory Title Mouse Up"));

	//마우스가 떼지면 드래그 위젯 해제 
	UMainWidget* MainWidget = Cast<UMainWidget>(mParentWidget);

	if (MainWidget)
	{
		MainWidget->SetDragWidget(nullptr);
	}
}

//FEventReply UPlayerInventoryFixedSlot::TitleMouseMove(FGeometry MyGeoMetry, const FPointerEvent& MouseEvent)
//{
//}


//void UPlayerInventoryFixedSlot::OnItemClick(UObject* ClickedItem)
//{
//}
//
//void UPlayerInventoryFixedSlot::OnItemDoubleClick(UObject* ClickedItem)
//{
//}
//
//void UPlayerInventoryFixedSlot::OnItemHovered(UObject* ClickedItem, bool IsHovered)
//{
//}
//
//void UPlayerInventoryFixedSlot::OnItemSelected(UObject* ClickedItem)
//{
//}


