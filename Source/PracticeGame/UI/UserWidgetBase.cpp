// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidgetBase.h"
#include "WidgetManager.h"

UUserWidgetBase::UUserWidgetBase(const FObjectInitializer& ObjectInitializer) :
	UUserWidget(ObjectInitializer)
{
}

void UUserWidgetBase::PlayWidgetAnimation(const FString& Name, float PlayStartTime, float PlaySpeed, bool Forward, bool RestoreState, int32 LoopCount)
{
	TObjectPtr<UWidgetAnimation>		Anim = mAnimMap.FindRef(Name);

	if (IsValid(Anim))
	{
		EUMGSequencePlayMode::Type PlayMode = EUMGSequencePlayMode::Forward;

		if (!Forward)
			PlayMode = EUMGSequencePlayMode::Reverse;

		PlayAnimation(Anim, PlayStartTime, LoopCount, PlayMode, PlaySpeed, RestoreState);
	}
}

void UUserWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if(bPrintLog)
		UE_LOG(TestGame, Warning, TEXT("NativeOnInitialized : %s"),
			*mWidgetName);

	//위젯 매니저에 현재 위젯을 등록한다
	CWidgetManager::GetInst()->AddWidget(mWidgetName, this);

}
void UUserWidgetBase::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativePreConstruct"));
}
void UUserWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeConstruct"));

	//위젯트리를 가지고 있는 클래스를 가져온다
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();

	//int32.AnimCount = WidgetClass->Animations.Num();

	for (auto& Anim : WidgetClass->Animations)
	{
		FString Name = Anim->GetName();

		//모든 애니메이션은 자동으로 이름 끝에 _INST 가 붙는다. 제거하자 
		Name.ReplaceInline(TEXT("_INST"), TEXT(""), ESearchCase::IgnoreCase);

		mAnimMap.Add(Name, Anim);
	}
}
void UUserWidgetBase::NativeDestruct()
{
	Super::NativeDestruct();
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeDestruct"));

	//현재 위젯을 위젯 매니저의 등록리스트에서 지워준다
	CWidgetManager::GetInst()->DeleteWidget(mWidgetName);

}
void UUserWidgetBase::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeTick"));
}

int32 UUserWidgetBase::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 Result = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativePaint"));

	return Result;
}

FReply UUserWidgetBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FReply Result = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnFocusReceived"));

		return Result;
}
void UUserWidgetBase::NativeOnFocusLost(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusLost(InFocusEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnFocusLost"));
}
void UUserWidgetBase::NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent)
{
	Super::NativeOnFocusChanging(PreviousFocusPath, NewWidgetPath, InFocusEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnFocusChanging"));
}
void UUserWidgetBase::NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnAddedToFocusPath(InFocusEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnAddedToFocusPath"));
}
void UUserWidgetBase::NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent)
{
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnRemovedFromFocusPath"));
}
FNavigationReply UUserWidgetBase::NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply)
{
	FNavigationReply Result = Super::NativeOnNavigation(MyGeometry, InNavigationEvent, InDefaultReply);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnNavigation"));

	return Result;
}
FReply UUserWidgetBase::NativeOnKeyChar(const FGeometry& InGeometry, const FCharacterEvent& InCharEvent)
{
	FReply Result = Super::NativeOnKeyChar(InGeometry, InCharEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnKeyChar"));

	return Result;
}
FReply UUserWidgetBase::NativeOnPreviewKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Result = Super::NativeOnPreviewKeyDown(InGeometry, InKeyEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnPreviewKeyDown"));

	return Result;
}
FReply UUserWidgetBase::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Result = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnKeyDown"));

	return Result;
}
FReply UUserWidgetBase::NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Result = Super::NativeOnKeyUp(InGeometry, InKeyEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnKeyUp"));

	return Result;
}
FReply UUserWidgetBase::NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent)
{
	FReply Result = Super::NativeOnAnalogValueChanged(InGeometry, InAnalogEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnAnalogValueChanged"));

	return Result;
}
FReply UUserWidgetBase::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnMouseButtonDown"));

	return Result;
}

FReply UUserWidgetBase::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);

	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnPreviewMouseButtonDown"));

	if (mParentWidget)
	{
		UUserWidgetBase* ParentWidget = Cast< UUserWidgetBase>(mParentWidget);

		if (ParentWidget)
		{
			ParentWidget->SetFocusWidget(this);
		}
	}

	return Result;
}
FReply UUserWidgetBase::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnMouseButtonUp"));

	return Result;
}
FReply UUserWidgetBase::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnMouseMove"));

	return Result;
}
void UUserWidgetBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnMouseEnter"));
}
void UUserWidgetBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnMouseLeave"));
}
FReply UUserWidgetBase::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnMouseWheel"));

	return Result;
}
FReply UUserWidgetBase::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnMouseButtonDoubleClick"));

	return Result;
}
void UUserWidgetBase::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry,InMouseEvent, OutOperation);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnDragDetected"));
}
void UUserWidgetBase::NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragEnter(InGeometry, InDragDropEvent, InOperation);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnDragEnter"));
}
void UUserWidgetBase::NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDragLeave(InDragDropEvent, InOperation);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnDragLeave"));
}
bool UUserWidgetBase::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool Result = Super::NativeOnDragOver(InGeometry, InDragDropEvent, InOperation);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnDragOver"));

	return Result;
}
bool UUserWidgetBase::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool Result = 	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnDrop"));

	return Result;
}
void UUserWidgetBase::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnDragCancelled"));
}
FReply UUserWidgetBase::NativeOnTouchGesture(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Result = Super::NativeOnTouchGesture(InGeometry, InGestureEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnTouchGesture"));

	return Result;
}
FReply UUserWidgetBase::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Result = Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnTouchStarted"));

	return Result;
}
FReply UUserWidgetBase::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Result = Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnTouchMoved"));

	return Result;
}
FReply UUserWidgetBase::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	FReply Result = Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnTouchEnded"));

	return Result;
}
FReply UUserWidgetBase::NativeOnMotionDetected(const FGeometry& InGeometry, const FMotionEvent& InMotionEvent)
{
	FReply Result = Super::NativeOnMotionDetected(InGeometry, InMotionEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnMotionDetected"));

	return Result;
}
FReply UUserWidgetBase::NativeOnTouchForceChanged(const FGeometry& MyGeometry, const FPointerEvent& TouchEvent)
{
	FReply Result = Super::NativeOnTouchForceChanged(MyGeometry, TouchEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnTouchForceChanged"));

	return Result;
}
FCursorReply UUserWidgetBase::NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent)
{
	FCursorReply Result = Super::NativeOnCursorQuery(InGeometry, InCursorEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnCursorQuery"));

	return Result;
}
FNavigationReply UUserWidgetBase::NativeOnNavigation(const FGeometry& InGeometry, const FNavigationEvent& InNavigationEvent)
{
	FNavigationReply Result = Super::NativeOnNavigation(InGeometry, InNavigationEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnNavigation"));

	return Result;
}
void UUserWidgetBase::NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent)
{
	Super::NativeOnMouseCaptureLost(CaptureLostEvent);
	if (bPrintLog)
		UE_LOG(TestGame, Display, TEXT("NativeOnMouseCaptureLost"));
}