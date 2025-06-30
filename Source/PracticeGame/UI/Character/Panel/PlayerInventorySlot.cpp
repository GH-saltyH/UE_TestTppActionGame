// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInventorySlot.h"
#include "../InvItemData.h"
#include "../../../Player/InventoryComponent.h"
#include "../../MainGame/ItemSlotDragDropOperation.h"

UPlayerInventorySlot::UPlayerInventorySlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


void UPlayerInventorySlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//������  Icon ��� ����ֱ� 
	mSlotIcon = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	mSlotBorder = Cast<UImage>(GetWidgetFromName(TEXT("Icon_Border")));

}

FReply UPlayerInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Result;
	Result.NativeReply= Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	//���콺 ���� ��ư���� üũ�� -> �巡�� ����
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		if (mInventoryComponent)
		{
			if (mInventoryComponent->CheckItem(mSlotIndex))
			{
				//�巡�װ� ���۵Ǿ����� �˷��ش�
				Result = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
			}
		}
	} 
	return Result.NativeReply;
}

void UPlayerInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	//OutOperation �� ������ ���۷��̼��� ����־��ش�
	if (!OutOperation)
	{

		// ��������� �巡�׵�� ���۷��̼� ����ƮŬ���� ������ ������ ���۷��̼��� ������ش�
		UItemSlotDragDropOperation* DragSlot =
			Cast<UItemSlotDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemSlotDragDropOperation::StaticClass()));

		DragSlot->mPrevSlotIndex = mSlotIndex;

		//�巡�� ���� ������ �����Ǿ��� -> ������ �������� ������ ������� ���� �ƴϴ�. ���� ������ ������ �Ѵ�.
		OutOperation = DragSlot;
		
		//�巡�� �߿� �������� ������ ����� �ð������� Ȯ���� �� �ְ� �Ѵ�
		if (IsValid(DragSlot))
		{
			//mDragWidgetClass �� �������Ʈ ��밡���̹Ƿ� ������ ��������. �ڽ��� mDragWidgetClass �� �����س��ƾ��Ѵ�. �������Ʈ���� ���� �����ϴ�
			UPlayerInventorySlot* DragWidget = CreateWidget<UPlayerInventorySlot>(GetWorld(), mDragWidgetClass);

			//�巡�������� ��������� ���� �巡�� ���� �������� �������Բ� �������ش�
			if (DragWidget)
			{
				DragWidget->SetInventoryComponent(mInventoryComponent);
				DragWidget->SetSlotIndex(mSlotIndex);

				//�������� �޾ƿ´�
				UInvItemData* Item = mInventoryComponent->GetItem(mSlotIndex);

				//�ش� ���������� DragWidget �� ������ �����Ѵ�
				DragWidget->ChangeItem(Item);
				DragWidget->mSlotBorder->SetVisibility(ESlateVisibility::Collapsed);
				DragWidget->mSlotIcon->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.4f));

				//DefaultDragVisual �� ���Ե巡�׵�ӿ��۷��̼ǿ��� �����ϴ� 
				// �巡�� ���־� �����̴� 
				// ���⿡ UI �� �����ϸ� ���̰� �ȴ�
				DragSlot->DefaultDragVisual = DragWidget;

				//Pivot �� �巡���� �߽����� �����Ѵ�
				//���콺 Ŭ�������� �߽����� Drag �� �����Ѵ�.
				DragSlot->Pivot = EDragPivot::MouseDown;
			}
		}
	}
}

bool UPlayerInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool Result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);


	//�������� ���ҵǾ�� �ϴ��� �Ǵ��ϰ� �����Ѵ�

	//Ŀ���� ���۷��̼����� ����ȯ�Ѵ�
	UItemSlotDragDropOperation* DragSlot = Cast< UItemSlotDragDropOperation>(InOperation);

	//�巡�׷� ��ġ��ȯ�� �ؾ� �Ѵٸ� ��ġ�� ��ȯ��Ų�� -> TArray �� SwapItem
	if (DragSlot)
	{
		if(mSlotIndex != DragSlot->mPrevSlotIndex)
		{
			mInventoryComponent->SwapItem(DragSlot->mPrevSlotIndex, mSlotIndex);
			return true;
		}
	}
	return false;
}


//�Ʒ��� �Լ��� ��������Ʈ�� ���� ���̴�.
void UPlayerInventorySlot::ChangeItem(UInvItemData* Item)
{
	//�������� �ִ� ���
	if (Item)
	{
		mSlotIcon->SetBrushFromTexture(Item->GetMasterData().Icon);
		mSlotIcon->SetVisibility(ESlateVisibility::Visible);
	}
	//�������� �����ϴ� ���
	else
	{
		mSlotIcon->SetBrushFromTexture(nullptr);
		mSlotIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}

