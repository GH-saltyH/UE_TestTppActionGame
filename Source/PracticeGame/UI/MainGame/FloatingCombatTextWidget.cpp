// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingCombatTextWidget.h"


UFloatingCombatTextWidget::UFloatingCombatTextWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	//UUserWidget 으로 상속 바꿨기 때문에 해제
	//mWidgetName = TEXT("FloatingCombatTextWidget");
}

void UFloatingCombatTextWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mDamageText = Cast<UTextBlock>(GetWidgetFromName(TEXT("FCTOutText")));

	//UI 에서 요소들의 주소를 가져온다
	/*mNPCName = Cast<UTextBlock>(GetWidgetFromName(TEXT("NPCName")));
	mHPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HPProgress")));*/
}

void UFloatingCombatTextWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	//1. screen 영역기준 원래 생성된 타겟의 위치 갱신하기
	// 현재 screen 으로 전환해서 main UI 의캔버스에 뿌리고 있으므로 이 과정이 있어야 카메라 이동으로 뷰포트가 변경되어도 대상 객체에 유지된다
	if (!mTrackedActor)
		return;

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
		return;

	FVector WorldLocation = mTrackedActor->mMesh->GetSocketLocation(TEXT("HealthBar"));

	FVector2D ScreenPos;
	bool bProjected = UGameplayStatics::ProjectWorldToScreen(PC, WorldLocation, ScreenPos, true);

	//FVector2D ViewportSize;
	//GEngine->GameViewport->GetViewportSize(ViewportSize);

	//FVector2D CanvasSize = UWidgetLayoutLibrary::GetViewportSize(this);

	//FVector2D Normalized = ScreenPos / ViewportSize;
	//FVector2D FinalCanvasPos = Normalized * CanvasSize;

	/*this->SetRenderTranslation(ScreenPos);*/

	if (bProjected)
	{
		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Slot))
		{
			////해상도 - UMG불일치로 인한 밀림문제 해결을 위한 앵커 0, 0 설정
			//CanvasSlot->SetAnchors(FAnchors(0, 0));
			//CanvasSlot->SetAlignment(FVector2D(0, 0));
			//CanvasSlot->SetPosition(ScreenPos);

			//CanvasSlot->SetRenderTranslation
		}
	}

	//2. 지속시간에 걸쳐 애니메이션 재생시키기 
	mElapsedTime += InDeltaTime;

	// 이동 시키기 mDirection 이 위쪽 xx도 범위 안에서 한 곳으로 랜덤으로 정해졌기 떄문에 그 방향으로 계속 갈것이다
	//방향만 있는 단위 벡터기 때문에 InDeltaTime 을 곱해서 크기로 만들어준다 -> 이동거리
	FVector2D Offset = mDirection * InDeltaTime;
	SetRenderTranslation(GetRenderTransform().Translation + Offset);
	UE_LOG(TestGame, Warning, TEXT("FCT :: Current Render Position [%s]"), *GetRenderTransform().Translation.ToString());
	//페이드 아웃 -> 지속시간이 경과할 때마다 0으로 수렴하도록
	float Alpha = 1.0f - (mElapsedTime / mLifeTime);

	//정해진 시간에 도달하면 위젯 제거 
	if (mElapsedTime >= mLifeTime)
	{
		RemoveFromParent();
	}
}

void UFloatingCombatTextWidget::SetNPCName(const FString& NewName)
{
	UE_LOG(TestGame, Warning, TEXT("NPC : SetNPCName"));
	//mNPCName->SetText(FText::FromString(NewName));
}

void UFloatingCombatTextWidget::SetHPBar(float HP, float MaxHP)
{
	UE_LOG(TestGame, Warning, TEXT("NPC : SetHPBar"));
	//mHPBar->SetPercent(HP / MaxHP);
}

void UFloatingCombatTextWidget::InitFCT(const FText& OutText, ANPC* TargetActor)
{
	mTrackedActor = TargetActor;

	mDamageText->SetText(OutText);
	UE_LOG(TestGame, Warning, TEXT("FCT :: SetText Complete,  OutPut Text = %s"), *OutText.ToString());
}

void UFloatingCombatTextWidget::PlayFloatAnimation()
{
	//업벡터 기준 +- 30 도 (불꽃이 위로만 튀듯이 ㅋㅋ) 해보자
	float AngleOffsetDeg = FMath::FRandRange(-30.f, 30.f);
	float AngleRadian = FMath::DegreesToRadians(AngleOffsetDeg);   //라디안변환!  끝

	//업벡터 기준점 잡아주기 -> 화면의 위방향
	FVector2D	DirectionBase = FVector2D(0, -1.f);
	//속도 계수 하나 지정해두자
	float SpeedMultiplier = 50.f;		
	//실제 방향 
	mDirection = DirectionBase.GetRotated(FMath::RadiansToDegrees(AngleRadian)) * SpeedMultiplier;

	SetRenderOpacity(1.0f);
	mElapsedTime = 0.f;
	UE_LOG(TestGame, Warning, TEXT("FCT :: PlayFloatAnimation Set"));
}

