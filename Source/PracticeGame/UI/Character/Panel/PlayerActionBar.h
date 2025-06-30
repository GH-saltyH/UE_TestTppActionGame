// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../GameInfo.h"
#include "../../UserWidgetBase.h"
#include "PlayerActionBar.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UPlayerActionBar : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UPlayerActionBar(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void NativeOnInitialized() override;
};
