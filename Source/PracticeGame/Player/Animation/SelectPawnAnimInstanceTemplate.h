// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Animation/AnimInstance.h"
#include "SelectPawnAnimInstanceTemplate.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ESelectAnim : uint8
{
	Idle,
	Select
};

UCLASS()
class PRACTICEGAME_API USelectPawnAnimInstanceTemplate : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USelectPawnAnimInstanceTemplate();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ESelectAnim mAnim;

public:
	void ChangeAnim(ESelectAnim NewAnim)
	{
		mAnim = NewAnim;
	}
	
	//열거형을 기준으로 트렌지션을 적용하는 노티파이 -> Idle 로 돌아가게끔 하기 위해 정의했다.
	UFUNCTION()
	void AnimNotify_SelectTransition();
};
