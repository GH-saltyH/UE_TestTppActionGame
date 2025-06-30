// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "BossDummy.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API ABossDummy : public AMonster
{
	GENERATED_BODY()
	

public:
	ABossDummy();

protected:
	virtual void BeginPlay()	override;

public:
	virtual void Tick(float DeltaTime)	override;

	virtual void NormalAttack() override;
	virtual void SpecialAttack() override;
};
