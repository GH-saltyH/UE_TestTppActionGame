// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../NPC.h"
#include "NPCMerchant.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API ANPCMerchant : public ANPC
{
	GENERATED_BODY()

public:
	ANPCMerchant();
	
public:
	virtual void OnClicked();
	virtual void OnFocusOut();
	
};
