// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "RogueProjectileA.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class PRACTICEGAME_API ARogueProjectileA : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARogueProjectileA();

private:
	//������Ʈ�����Ʈ ������Ʈ �ּ�
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> mProjectMovement;
	//����ƽ �޽� ������Ʈ �ּ�
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UStaticMeshComponent> mStaticMesh;
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UBoxComponent>	mCollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> mNiagaraComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> mNiagaraSystem;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UNiagaraSystem> mImpactVFX;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	TObjectPtr<UParticleSystem> mArrowDisappearVFX;
	
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USphereComponent>	mCollisionComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitProjectile(FVector Direction, float Speed);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,		 
		UPrimitiveComponent* OtherComp, 
		FVector NormalImpulse, 
		const FHitResult& Hit);
};
