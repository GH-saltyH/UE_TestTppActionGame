// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PawnMovementComponent.h"
#include "AIMovement.generated.h"

UCLASS()
class PRACTICEGAME_API UAIMovement : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UAIMovement();

protected:
	FVector	mPendingInput;

	UPROPERTY(VisibleAnywhere)
	FVector	mVelocity;

	UPROPERTY(EditAnywhere)
	float mMaxSpeed = 500.f;

	UPROPERTY(EditAnywhere)
	float mAcceleration = 1500.f;

	UPROPERTY(EditAnywhere)
	float	mGravityAccel = -980.f;


	//길찾기 멤버
		//이동할 목표지점
	FVector mTargetLocation;
	//목표지점으로 가는 경로들이 들어갈 공간
	TArray<FVector>	mPathArray;
	//목표지점으로 가는 경로의 인덱스
	int32		mCurrentPathIndex = 0;


	/** Deceleration applied when there is no input (rate of change of velocity) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatingPawnMovement)
	float mDeceleration = 8000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatingPawnMovement, meta = (ClampMin = "0", UIMin = "0"))
	float mTurningBoost = 8.f;

	UPROPERTY(Transient)
	uint32 mbPositionCorrected : 1;


public:
	void SetMaxSpeed(float NewMaxSpeed)
	{
		mMaxSpeed = NewMaxSpeed;
	}

	void SetTarget(AActor* Target);
	void SetTarget(const FVector& Target);

public:
	virtual void AddInputVector(FVector WorldVector, bool bForce = false);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	void FindPathToTarget();
	void FollowPath(const FVector& CurrentLocation, float DeltaTime);
	void Move(float DeltaTime);

protected:
	virtual void ApplyControlInputToVelocity(float DeltaTime);
	virtual bool LimitWorldBounds();
	virtual bool ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotation) override;

public:
	void ClearPath();

};