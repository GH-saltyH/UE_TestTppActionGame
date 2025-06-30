// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "NPCMerchant.h"
#include "DummyMerchantNPC.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API ADummyMerchantNPC : public ANPCMerchant
{
	GENERATED_BODY()

public:
	ADummyMerchantNPC();

public:
	virtual void OnClicked();
	virtual void OnConstruction(const FTransform& Transform);
	
};
