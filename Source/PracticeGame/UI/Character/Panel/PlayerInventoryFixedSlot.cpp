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

	//��ҵ� �޾Ƶα�
	mTitleBarBorder = Cast<UBorder>(GetWidgetFromName(TEXT("TitleBarBorder")));
	mButtonDragBar = Cast<UButton>(GetWidgetFromName(TEXT("TitleBarButton")));
	mButtonExit = Cast<UButton>(GetWidgetFromName(TEXT("ButtonExit")));

	//�ݱ� ��ư ��������Ʈ �Լ� ���ε�
	mButtonExit->OnClicked.AddDynamic(this, &UPlayerInventoryFixedSlot::ButtonCloseOnClick);

	mButtonDragBar->OnPressed.AddDynamic(this, &UPlayerInventoryFixedSlot::TitleMouseDown);
	mButtonDragBar->OnReleased.AddDynamic(this, &UPlayerInventoryFixedSlot::TitleMouseUp);
}

void UPlayerInventoryFixedSlot::ButtonCloseOnClick()
{
	//â�ݱ� ��ư
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

		//���Թ�ȣ ���� �� ������ ���Ծ����� �ʱ�ȭ
		InvSlot->SetSlotIndex(i);
		InvSlot->ChangeItem(nullptr);
		//slot ���� ������Ʈ �ּҸ� �����ؼ� ����ϰ� �Ѵ�(��� ������ ó���ϱ� ����
		InvSlot->SetInventoryComponent(Inv);
		mInvSlots.Add(InvSlot);
	}

	// ��Ƽĳ��Ʈ ��������Ʈ ���ε� �� 
	Inv->mInvChangeDelegate.AddUObject(this, &UPlayerInventoryFixedSlot::ChangeItem);
	Inv->mChangeGoldDelegate.AddUObject(this, &UPlayerInventoryFixedSlot::SetGold);
}

void UPlayerInventoryFixedSlot::ChangeItem(UInvItemData* Item, int32 Index)
{
	//������ ���� �ݿ��ϱ� -> ���� ������ �����̾ƴ� ���� ������������ ó���ؾ� �ϹǷ� �ش� ������ ChangeItem ���� ó���ϵ��� �����Ѵ�
	mInvSlots[Index]->ChangeItem(Item);
}


void UPlayerInventoryFixedSlot::TitleMouseDown()
{

	UE_LOG(TestGame, Warning, TEXT("Inventory Title Mouse Down"));

	//���������� �巡�� �������� �� ������ �����Ѵ�.  
	//�������� �������� �巡�� ������ ���õ� ����� �ϴµ� �����ϰ� �� ���̴�
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

	//���콺�� ������ �巡�� ���� ���� 
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


