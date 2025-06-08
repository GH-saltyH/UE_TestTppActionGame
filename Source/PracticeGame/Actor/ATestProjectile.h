// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ATestProjectile.generated.h"

UCLASS()
class PRACTICEGAME_API AATestProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AATestProjectile();

private:
	//������Ʈ�����Ʈ ������Ʈ �ּ�
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> mProjectMovement;
	//����ƽ �޽� ������Ʈ �ּ�
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mStaticMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent>	mCollisionBox;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnHit(
		AActor* SelfActor,
		AActor* OtherActor,
		FVector NormalImpulse,
		const FHitResult& Hit);
};
