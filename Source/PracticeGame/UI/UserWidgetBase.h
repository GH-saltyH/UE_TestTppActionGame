// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UIInfo.h"
#include "Blueprint/UserWidget.h"
#include "UserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UUserWidgetBase(const FObjectInitializer& ObjectInitializer);
	
protected:
	bool bPrintLog = false;
	TMap<FString, TObjectPtr<UWidgetAnimation>>		mAnimMap;
	AActor* mOwnerActor = nullptr;
	
	//상속받는 위젯들의 이름을 저장할 멤버이다.
	FString		mWidgetName;

	TObjectPtr<UUserWidget>						mParentWidget;
	TObjectPtr<UUserWidget>						mFocusedWidget;
	
public:
	void PlayWidgetAnimation(const FString& Name, float PlayStartTime = 0.f, float PlaySpeed = 1.f, bool Forward = true, bool RestoreState = false, int32 LoopCount = 1);

	void SetParentWidget(UUserWidget* Parent)
	{
		mParentWidget = Parent;
	}

	void SetFocusWidget(UUserWidget* TargetWidget)
	{
		//기존의 포커스드 위젯은 0으로 내려준다, 지금 선택하려는 위젯과 다른경우만
		if (mFocusedWidget && mFocusedWidget == TargetWidget)
		{
			UCanvasPanelSlot* CanvasSlot =
				Cast<UCanvasPanelSlot>(mFocusedWidget->Slot);

			if (CanvasSlot)
			{
				CanvasSlot->SetZOrder(0);
			}
		}

		//새로운 포커스드 위젯을 설정한다 
		if (mFocusedWidget)
		{
			mFocusedWidget = TargetWidget;

			UCanvasPanelSlot* CanvasSlot =
				Cast<UCanvasPanelSlot>(mFocusedWidget->Slot);

			if (CanvasSlot)
			{
				CanvasSlot->SetZOrder(999);
			}
		}
	}

protected:
	virtual void NativeOnInitialized();		
	virtual void NativePreConstruct();
	virtual void NativeConstruct();
	virtual void NativeDestruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent);
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent);
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent);
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent);
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent);
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply);
	virtual FReply NativeOnKeyChar(const FGeometry& InGeometry, const FCharacterEvent& InCharEvent);
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent);
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation);
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation);
	virtual FReply NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent);
	virtual FReply NativeOnMotionDetected(const FGeometry& InGeometry, const FMotionEvent& InMotionEvent);
	virtual FReply NativeOnTouchForceChanged(const FGeometry& MyGeometry, const FPointerEvent& TouchEvent);
	virtual FCursorReply NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent);
	virtual FNavigationReply NativeOnNavigation(const FGeometry& InGeometry, const FNavigationEvent& InNavigationEvent);
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent);
};
