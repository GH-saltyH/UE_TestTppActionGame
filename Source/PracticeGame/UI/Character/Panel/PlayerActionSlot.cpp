// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerActionSlot.h"
#include "../InvItemData.h"
#include "../../../Player/InventoryComponent.h"
#include "../../MainGame/ItemSlotDragDropOperation.h"

UPlayerActionSlot::UPlayerActionSlot(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}


void UPlayerActionSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//위젯의  Icon 요소 들고있기 
	mSlotIcon = Cast<UImage>(GetWidgetFromName(TEXT("Icon")));
	mSlotBorder = Cast<UImage>(GetWidgetFromName(TEXT("Icon_Border")));

}

FReply UPlayerActionSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Result;
	Result.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	////마우스 왼쪽 버튼인지 체크함 -> 드래그 시작
	//if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	//{
	//	if (mSkillComponent)
	//	{
	//		if (mSkillComponent->CheckItem(mSlotIndex))
	//		{
	//			//드래그가 시작되었음을 알려준다
	//			Result = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	//		}
	//	}
	//}
	return Result.NativeReply;
}

void UPlayerActionSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	//OutOperation 이 없으면 오퍼레이션을 집어넣어준다
	if (!OutOperation)
	{

		// 사용자정의 드래그드롭 오퍼레이션 디폴트클래스 정보를 가지고 오퍼레이션을 만들어준다
		UItemSlotDragDropOperation* DragSlot =
			Cast<UItemSlotDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UItemSlotDragDropOperation::StaticClass()));

		DragSlot->mPrevSlotIndex = mSlotIndex;

		//드래그 중인 슬롯이 생성되었다 
		OutOperation = DragSlot;

		//이제 드래그 중에 보여지는 위젯을 만들어 시각적으로 확인할 수 있게 한다
		if (IsValid(DragSlot))
		{
			//블루프린트 사용가능이므로 UI_PlayerInvSlot 의 이벤트 그래프에서, mDragWidgetClass 에  UI_PlayerInvSlot 을 할당하면된다
			UPlayerActionSlot* DragWidget = CreateWidget<UPlayerActionSlot>(GetWorld(), mDragWidgetClass);

			//드래그위젯이 만들어지면 현재 드래그 중인 아이템이 보여지게끔 설정해준다
			if (DragWidget)
			{
				DragWidget->SetSkillComponent(mSkillComponent);
				DragWidget->SetSlotIndex(mSlotIndex);

				//아이템을 받아온다
				//USkillListData* Skill = mSkillComponent->GetSkill(mSlotIndex);

				//해당 아이템으로 DragWidget 의 외형을 설정한다
				/*DragWidget->ChangeSkill(Skill);
				DragWidget->mSlotBorder->SetVisibility(ESlateVisibility::Collapsed);
				DragWidget->mSlotIcon->SetColorAndOpacity(FLinearColor(1, 1, 1, 0.4f));*/

				//DefaultDragVisual 은 슬롯드래그드롭오퍼레이션에서 제공하는 
				// 드래깅 비주얼 공간이다 
				// 여기에 UI 를 지정하면 보이게 된다
				DragSlot->DefaultDragVisual = DragWidget;

				//Pivot 은 드래그의 중심점을 지정한다
				//마우스 클릭지점을 중심으로 Drag 를 진행한다.
				DragSlot->Pivot = EDragPivot::MouseDown;
			}
		}
	}
}

bool UPlayerActionSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool Result = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	////아이템이 스왑되어야 하는지 판단하고 스왑한다

	////커스텀 오퍼레이션으로 형변환한다
	//UItemSlotDragDropOperation* DragSlot = Cast< UItemSlotDragDropOperation>(InOperation);

	////드래그로 위치변환을 해야 한다면 위치를 변환시킨다 -> TArray 의 SwapItem
	//if (DragSlot)
	//{
	//	if (mSlotIndex != DragSlot->mPrevSlotIndex)
	//	{
	//		mInventoryComponent->SwapItem(DragSlot->mPrevSlotIndex, mSlotIndex);
	//		return true;
	//	}
	//}
	return false;
}


//아래의 함수는 델리게이트로 콜할 것이다.
void UPlayerActionSlot::ChangeSkill(USkillListData* Skill)
{
	//아이템이 있는 경우
	if (Skill)
	{
		mSlotIcon->SetBrushFromTexture(Skill->GetSkillData().Icon);
		mSlotIcon->SetVisibility(ESlateVisibility::Visible);
	}
	//아이템을 제거하는 경우
	else
	{
		mSlotIcon->SetBrushFromTexture(nullptr);
		mSlotIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}





