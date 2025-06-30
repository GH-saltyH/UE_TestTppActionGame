// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include	"../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "LootAbleActorDummy.generated.h"

UCLASS()
class PRACTICEGAME_API ALootAbleActorDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootAbleActorDummy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
