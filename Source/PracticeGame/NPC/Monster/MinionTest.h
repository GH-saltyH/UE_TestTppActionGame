// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "MinionTest.generated.h"

UCLASS()
class PRACTICEGAME_API AMinionTest : public AMonster
{
	GENERATED_BODY()
	
public:
	AMinionTest();

protected:
	virtual void BeginPlay()	override;

public:
	virtual void Tick(float DeltaTime)	override;
	
	virtual void NormalAttack() override;
	
};
