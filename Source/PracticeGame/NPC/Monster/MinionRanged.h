// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "Monster.h"
#include "MinionRanged.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API AMinionRanged : public AMonster
{
	GENERATED_BODY()
	

public:
	AMinionRanged();

protected:
	virtual void BeginPlay()	override;

public:
	virtual void Tick(float DeltaTime)	override;

	virtual void NormalAttack() override;

};