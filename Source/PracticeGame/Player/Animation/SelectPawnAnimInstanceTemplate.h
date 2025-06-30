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
	
	//�������� �������� Ʈ�������� �����ϴ� ��Ƽ���� -> Idle �� ���ư��Բ� �ϱ� ���� �����ߴ�.
	UFUNCTION()
	void AnimNotify_SelectTransition();
};
