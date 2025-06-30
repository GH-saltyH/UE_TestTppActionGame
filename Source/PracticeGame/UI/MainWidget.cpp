// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Character/Panel/PlayerSkillPanelWidget.h"
//리스트 뷰 형식 인벤토리
//#include "Character/Panel/PlayerInvPanelWidget.h"
#include "Character/Panel/PlayerInventoryFixedSlot.h"
#include "MainGame/NPC/Merchant/TradePanelWidget.h"
#include "../QuestSystem/QuestWidget.h"
#include "MainGame/FloatingCombatTextWidget.h"

UMainWidget::UMainWidget(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	mWidgetName = TEXT("MainWidget");

	static ConstructorHelpers::FClassFinder<UUserWidget> FCTAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Combat/UI_FloatingCombatText.UI_FloatingCombatText_C'"));
	if (FCTAsset.Succeeded())
		mFCTWidgetClass = FCTAsset.Class;
}

void UMainWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mCharInfoWidget = Cast<UCharacterInfoWidget>(GetWidgetFromName(TEXT("UI_CharacterInfoWidget")));
	mSelectedNPCInfoWidget = Cast<USelectedNPCInfoWidget>(GetWidgetFromName(TEXT("UI_SelectedNPCInfo")));
	mPlayerSkillPanelWidget = Cast<UPlayerSkillPanelWidget>(GetWidgetFromName(TEXT("UI_PlayerSkillPanelWidget")));
	mPlayerInvPanelWidget = Cast<UPlayerInventoryFixedSlot>(GetWidgetFromName(TEXT("UI_PlayerInvFixed")));
	mTradePanelWidget = Cast<UTradePanelWidget>(GetWidgetFromName(TEXT("UI_TradePanelWidget")));
	mQuestWidget = Cast<UQuestWidget>(GetWidgetFromName(TEXT("UI_QuestWidget")));

	//FCT
	mFCTArea = Cast<UCanvasPanel>(GetWidgetFromName(TEXT("CanvasFHD")));

	mCharInfoWidget->SetParentWidget(this);
	mSelectedNPCInfoWidget->SetParentWidget(this);
	mPlayerSkillPanelWidget->SetParentWidget(this);
	mPlayerInvPanelWidget->SetParentWidget(this);
	mTradePanelWidget->SetParentWidget(this);
	mQuestWidget->SetParentWidget(this);


	mButtonSkill = Cast<UButton>(GetWidgetFromName(TEXT("ButtonSkill")));
	mButtonInv = Cast<UButton>(GetWidgetFromName(TEXT("ButtonInv")));

	mButtonSkill->OnClicked.AddDynamic(this, &UMainWidget::ButtonSkillOnClicked);
	mButtonInv->OnClicked.AddDynamic(this, &UMainWidget::ButtonInvOnClicked);

	
}

void UMainWidget::SetDragWidget(UUserWidget* DragWidget)
{
	//기존에 드래그 되던 위젯이 있을 경우 zOrder 를 0 으로 되돌려준다
	if (mDragWidget)
	{
		//Z 오더 최상위
		UCanvasPanelSlot* CanvasSlot =
			Cast<UCanvasPanelSlot>(mDragWidget->Slot);
		if (CanvasSlot)
		{
			CanvasSlot->SetZOrder(0);
		}
	}

	mDragWidget = DragWidget;
	//지금 드래그 중인 위젯의 ZOrder 를 가장 높게 설정한다
	if (mDragWidget)
	{
		//현재 뷰포트에서의 마우스 포지션을 얻어온다 FVector2D
		mDragPrevPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		UCanvasPanelSlot* CanvasSlot = Cast< UCanvasPanelSlot>(mDragWidget->Slot);

		if (CanvasSlot)
		{
			CanvasSlot->SetZOrder(999);
		}
	}
}


//드래그를 반영한다 
void UMainWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//마우스 드래그 기능 구현 -> 틱으로 매프레임 이동을 보여주게 하자

	if (mDragWidget)
	{
		//현재 메인 위젯은 전체 화면을 덮고 있기 때문에  Viewport 의 포지션 값을 받아서 처리하는 것이 효과적이다
		FVector2D		MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		//이전 위치로부터 이동한 양을 구한다 
		FVector2D		MouseOffset = MousePos - mDragPrevPosition;

		//이전포지션은 현재 포지션으로 갱신해준다
		mDragPrevPosition = MousePos;

		//구해진 마우스 이동량 만큼 현재 이동중인 위젯을 이동한다 
		UCanvasPanelSlot* CanvasSlot = Cast< UCanvasPanelSlot>(mDragWidget->Slot);
		if (CanvasSlot)
		{
			//현재 드래그 중인 위젯의 포지션을 얻는다
			FVector2D	WidgetPos = CanvasSlot->GetPosition();
			//현재 포지션에 오프셋을 더한 값으로 바꾼다
			CanvasSlot->SetPosition(WidgetPos + MouseOffset);
		}
	}
}

FReply UMainWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Result = Super::NativeOnMouseMove(InGeometry, InMouseEvent);
	UE::Slate::FDeprecateVector2DResult AbsolutePos = InGeometry.GetAbsolutePosition();

	//UE_LOG(TestGame, Warning, TEXT("MainWidget - OnMouseMove   [%0.2f, %0.2f]"), AbsolutePos.X, AbsolutePos.Y);
	return Result;
}

void UMainWidget::ButtonSkillOnClicked()
{
	UE_LOG(TestGame, Warning, TEXT("ButtonSkill : On Clicked!"));
	if (!mPlayerSkillPanelWidget)
		return;

	if (mPlayerSkillPanelWidget->IsVisible())
	{
		mPlayerSkillPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
		//UScaleBox* SkillBox = Cast<UScaleBox>(GetWidgetFromName(TEXT("Scale_Skill")));
		//if (SkillBox)
		//{
		//	SkillBox->SetVisibility(ESlateVisibility::Collapsed);
		//}
	}
	else
	{
		mPlayerSkillPanelWidget->SetVisibility(ESlateVisibility::Visible);
		//UScaleBox* SkillBox = Cast<UScaleBox>(GetWidgetFromName(TEXT("Scale_Skill")));
		//if (SkillBox)
		//{
		//	SkillBox->SetVisibility(ESlateVisibility::Visible);
		//}
	}
}

void UMainWidget::ButtonInvOnClicked()
{
	UE_LOG(TestGame, Warning, TEXT("ButtonInv : On Clicked!"));

	if (!mPlayerInvPanelWidget)
		return;

	if (mPlayerInvPanelWidget->IsVisible())
	{
		mPlayerInvPanelWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		mPlayerInvPanelWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainWidget::OpenTradePanel()
{
	if (!mTradePanelWidget)
	{
		UE_LOG(TestGame, Warning, TEXT("No TradePanel Found!"));
		return;
	}

	if (mTradePanelWidget->IsVisible())
		return;

	UE_LOG(TestGame, Warning, TEXT("TradePanel Opened!"));
	mTradePanelWidget->SetVisibility(ESlateVisibility::Visible);
	
}

void UMainWidget::CloseTradePanel()
{
	if (!mTradePanelWidget)
	{
		UE_LOG(TestGame, Warning, TEXT("No TradePanel Found!"));
		return;
	}


	if (mTradePanelWidget->IsVisible())
	{
		UE_LOG(TestGame, Warning, TEXT("No TradePanel Collapsed!"));
	mTradePanelWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

}

void UMainWidget::SpawnFloatingText(float Damage, ANPC* TargetActor)
{
	if (!mFCTWidgetClass)
	{
		UE_LOG(TestGame, Warning, TEXT("FCT::SpawnFloatingText  --  Terminated :: mFCTWidgetClass is nullptr!"));
		return;
	}

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		UE_LOG(TestGame, Warning, TEXT("FCT::SpawnFloatingText  --  Terminated :: Failed to Get PlayerController!"));
		return;
	}

	//타겟 본위치 구하기
	FVector	 WorldLocation = TargetActor->mMesh->GetSocketLocation(TEXT("HealthBar"));

	//월드 좌표 -> 스크린
	FVector2D		ScreenPos;
	if (!UGameplayStatics::ProjectWorldToScreen(PC, WorldLocation, ScreenPos, true))
	{
		UE_LOG(TestGame, Warning, TEXT("FCT::SpawnFloatingText  --  Terminated :: Failed to Convert Screen Position!"));
		return;
	}

	//FCT 위젯을 동적으로 생성한다
	UFloatingCombatTextWidget* FCT = CreateWidget<UFloatingCombatTextWidget>(GetWorld(), mFCTWidgetClass);
	if (!FCT)
	{
		UE_LOG(TestGame, Warning, TEXT("FCT::SpawnFloatingText  --   Terminated :: Failed to CreateWidget [UFloatingCombatTextWidget]"));
		return;
	}

	FCT->InitFCT(FText::AsNumber(Damage), TargetActor);
	
	if (UCanvasPanelSlot* PanelSlot = mFCTArea->AddChildToCanvas(FCT))
	{
		PanelSlot->SetAutoSize(true);
		PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
		PanelSlot->SetPosition(ScreenPos);
	}
	
	FCT->PlayFloatAnimation();

	UE_LOG(TestGame, Warning, TEXT("[FCT] Spawned at screen pos (%.0f, %.0f), damage: %.1f"),
		ScreenPos.X, ScreenPos.Y, Damage);
}