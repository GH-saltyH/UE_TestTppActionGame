// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UserWidgetBase.h"
#include "../../NPC/NPC.h"
#include "FloatingCombatTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UFloatingCombatTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFloatingCombatTextWidget(const FObjectInitializer& ObjectInitializer);

protected:
	TObjectPtr<UTextBlock> mDamageText;

	//애니메이션 정의
	//이동방향과 속도, 재생시간을 정의한다
	FVector2D	mDirection;
	float			mLifeTime = 1.0f;
	float			mElapsedTime = 0.f;

	//부착할 엑터 기억 (screen 좌표 Tick 에서 갱신하려면 기억해야함)
	TObjectPtr<ANPC>		mTrackedActor;
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	void SetNPCName(const FString& NewName);
	void SetHPBar(float HP, float MaxHP);
	
	void InitFCT(const FText& OutText, ANPC* TargetActor);
	void PlayFloatAnimation();
};
