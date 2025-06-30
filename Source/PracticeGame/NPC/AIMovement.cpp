// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMovement.h"
#include "NavigationSystem.h"		//길찾기 기능을 사용하기 위해 포함
#include "NavigationPath.h"			//길찾기 기능을 사용하기 위해 포함
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"		//EPathFollowingStatus 등을 사용하기 위해 포함

UAIMovement::UAIMovement()
{
	ResetMoveState();
}

void UAIMovement::SetTarget(AActor* Target)
{
	mTargetLocation = Target->GetActorLocation();

	UCapsuleComponent* Capsule =
		Cast<UCapsuleComponent>(Target->GetRootComponent());

	if (IsValid(Capsule))
	{
		mTargetLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
	}

	SetTarget(mTargetLocation);
}

void UAIMovement::SetTarget(const FVector& Target)
{
	mTargetLocation = Target;

	FindPathToTarget();
}

void UAIMovement::AddInputVector(FVector WorldVector,
	bool bForce)
{
	//Super::AddInputVector(WorldVector, bForce);
	mPendingInput += WorldVector;
}

void UAIMovement::TickComponent(float DeltaTime,
	enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!GetPawnOwner() || !UpdatedComponent)
		return;

	AAIController* AIController = Cast<AAIController>(GetController());

	if (AIController)
	{
		EPathFollowingStatus::Type State =
			AIController->GetPathFollowingComponent()->GetStatus();

		switch (State)
		{
		case EPathFollowingStatus::Idle:		//이동이 아닐 때 처리
			Move(DeltaTime);
			break;
		case EPathFollowingStatus::Waiting:		//길찾는 중
			break;
		case EPathFollowingStatus::Paused:		//이동 일시 정지.
			break;
		case EPathFollowingStatus::Moving:		//이동중
			Move(DeltaTime);
			mPendingInput = FVector::ZeroVector;
			break;
		}
	}

	return;

	//// 컴포넌트의 현재 발밑 위치를 얻어온다.
	//FVector	CurrentLocation = UpdatedComponent->GetComponentLocation();

	//UCapsuleComponent* Capsule =
	//	Cast<UCapsuleComponent>(UpdatedComponent);

	//// 발밑 위치를 구한다.
	//if (IsValid(Capsule))
	//{
	//	CurrentLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
	//}

	//if (mPathArray.Num() > 0 &&
	//	mCurrentPathIndex < mPathArray.Num())
	//{
	//	FollowPath(CurrentLocation, DeltaTime);
	//}

	//// GetClampedToMaxSize 함수는 인자로 들어가는 크기를 초과하는
	//// 경우 벡터의 최대 크기에 맞춰 크기만 줄이고 방향은 유지한 새로운
	//// 벡터를 반환한다.
	//FVector	Input = mPendingInput.GetClampedToMaxSize(1.0);

	//mPendingInput = FVector::ZeroVector;

	//FVector	HorizontalVelocity = FVector(mVelocity.X, mVelocity.Y, 0.0);

	//// 입력받은 벡터에 최대 속도를 곱하여 입력받은 방향으로의 최대속도벡터를
	//// 만든다.
	//FVector	MaxInputVelocity = Input * mMaxSpeed;

	//// 가속도 적용
	//HorizontalVelocity = FMath::VInterpConstantTo(HorizontalVelocity,
	//	MaxInputVelocity, DeltaTime, mAcceleration);

	//mVelocity.Z += mGravityAccel * DeltaTime;

	//mVelocity.X = HorizontalVelocity.X;
	//mVelocity.Y = HorizontalVelocity.Y;

	//FVector	MoveDelta = mVelocity * DeltaTime;

	//// 업데이트할 컴포넌트의 현재 위치와 이동할 위치의 거리를 구하여
	//// 이 거리가 위에서 구해준 현재 이동해야 하는 양보다 작거나 같을 경우
	//// 목표지점을 지나가게 되는 것이다.
	//if (mPathArray.Num() > 0 &&
	//	mCurrentPathIndex < mPathArray.Num())
	//{
	//	float	TargetDist = FVector::Dist(CurrentLocation,
	//		mPathArray[mCurrentPathIndex]);

	//	// 이 상태는 타겟을 지나치게 될 것이다.
	//	if (TargetDist <= MoveDelta.Size())
	//	{
	//		// 이동 방향을 가지고 와서 타겟과의 거리만큼 이동할 수 있게
	//		// 벡터를 만든다.
	//		MoveDelta = MoveDelta.GetSafeNormal() * TargetDist;
	//		++mCurrentPathIndex;
	//	}
	//}

	//FHitResult	Hit;
	//// GetComponentRotation : Component의 월드 회전 정보
	//// MoveDelta 만큼 이동하고 
	//SafeMoveUpdatedComponent(MoveDelta,
	//	UpdatedComponent->GetComponentRotation(), true,
	//	Hit);

	//// 다른 물체와 부딪혔을 경우
	//if (Hit.IsValidBlockingHit())
	//{
	//	//부딪힌 면의 바라보는 방향이 위쪽방향일 경우 아래쪽으로 부딪혔다고 판단하여 
	//	//중력에 의해 만들어진 속도를 0으로 초기화한다
	//	if (Hit.Normal.Z > 0.7f)
	//	{
	//		mVelocity.Z = 0.f;
	//	}

	//	//물체와 부딪혔기 때문에 부딪힌 
	//	//물체의 바라보는 방향을 이용하여 미끄러지게 이동하도록 처리한다
	//	SlideAlongSurface(MoveDelta, 1.f - Hit.Time, Hit.Normal,
	//		Hit);
	//}
}

void UAIMovement::FindPathToTarget()
{
	// 내비게이션 관리자를 얻어온다.
	UNavigationSystemV1* NavSystem =
		FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	// 내비게이션 시스템이 있고 이 무브먼트 콤포넌트가 업데이트할 컴포넌트가
	// 있을 경우
	if (NavSystem && UpdatedComponent)
	{
		FNavLocation	ProjectLocation;

		if (NavSystem->ProjectPointToNavigation(mTargetLocation,
			ProjectLocation))
		{
			// 현재 컴포넌트의 위치를 얻어온다.
			FVector	CurrentLocation = UpdatedComponent->GetComponentLocation();

			UCapsuleComponent* Capsule =
				Cast<UCapsuleComponent>(UpdatedComponent);

			// 발밑 위치를 구한다.
			if (IsValid(Capsule))
			{
				CurrentLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
			}

			UNavigationPath* NavPath =
				NavSystem->FindPathToLocationSynchronously(GetWorld(),
					CurrentLocation, ProjectLocation.Location);

			if (NavPath && NavPath->IsValid())
			{
				mPathArray = NavPath->PathPoints;
				//0번 인덱스가 이동이 없는 시작 지점이기 때문에 실제 이동이 이루어 져야 하는 
				//1번 인덱스 부터 시작하게 처리한다
				mCurrentPathIndex = 1;
			}
		}
	}
}

void UAIMovement::FollowPath(const FVector& CurrentLocation, float DeltaTime)
{
	// 끝까지 길을 찾아갔다는 의미이다.
	if (mCurrentPathIndex >= mPathArray.Num())
	{
		mPathArray.Empty();
		return;
	}

	// 이동할 지점을 얻어온다.
	FVector	TargetPoint = mPathArray[mCurrentPathIndex];

	//TargetPoint.Z = CurrentLocation.Z;
	// 경로를 구해줄 때 추가된 내비게이션 메시의 높이만큼을 차감한다
	TargetPoint.Z -= 10.f;

	// 이동 방향을 구한다.
	FVector	Dir = TargetPoint - CurrentLocation;

	// 크기가 1인 벡터로 변경한다.
	Dir = Dir.GetSafeNormal();

	mPendingInput += Dir;

	// 현재 프레임에 이동 방향으로 이동할 양을 구한다.
	//FVector	MoveDelta = Dir * mMaxSpeed * DeltaTime;
}

void UAIMovement::Move(float DeltaTime)
{
	const AController* Controller = PawnOwner->GetController();
	if (Controller && Controller->IsLocalController())
	{
		// apply input for local players but also for AI that's not following a navigation path at the moment
		if (Controller->IsLocalPlayerController() == true || Controller->IsFollowingAPath() == false || NavMovementProperties.bUseAccelerationForPaths)
		{
			ApplyControlInputToVelocity(DeltaTime);
		}
		// if it's not player controller, but we do have a controller, then it's AI
		// (that's not following a path) and we need to limit the speed
		else if (IsExceedingMaxSpeed(mMaxSpeed) == true)
		{
			Velocity = Velocity.GetUnsafeNormal() * mMaxSpeed;
		}

		LimitWorldBounds();
		mbPositionCorrected = false;

		// Move actor
		FVector Delta = Velocity * DeltaTime;

		if (!Delta.IsNearlyZero(1e-6f))
		{
			const FVector OldLocation = UpdatedComponent->GetComponentLocation();
			const FQuat Rotation = UpdatedComponent->GetComponentQuat();

			FHitResult Hit(1.f);
			SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

			if (Hit.IsValidBlockingHit())
			{
				HandleImpact(Hit, DeltaTime, Delta);
				// Try to slide the remaining distance along the surface.
				SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
			}

			// Update velocity
			// We don't want position changes to vastly reverse our direction (which can happen due to penetration fixups etc)
			if (!mbPositionCorrected)
			{
				const FVector NewLocation = UpdatedComponent->GetComponentLocation();
				Velocity = ((NewLocation - OldLocation) / DeltaTime);
			}
		}

		// Finalize
		UpdateComponentVelocity();
	}
	//// 컴포넌트의 현재 발밑 위치를 얻어온다.
	//FVector	CurrentLocation = UpdatedComponent->GetComponentLocation();

	//UCapsuleComponent* Capsule =
	//	Cast<UCapsuleComponent>(UpdatedComponent);

	//// 발밑 위치를 구한다.
	//if (IsValid(Capsule))
	//{
	//	CurrentLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
	//}

	//if (mPathArray.Num() > 0 &&
	//	mCurrentPathIndex < mPathArray.Num())
	//{
	//	FollowPath(CurrentLocation, DeltaTime);
	//}

	//// GetClampedToMaxSize 함수는 인자로 들어가는 크기를 초과하는
	//// 경우 벡터의 최대 크기에 맞춰 크기만 줄이고 방향은 유지한 새로운
	//// 벡터를 반환한다.
	//FVector	Input = mPendingInput.GetClampedToMaxSize(1.0);

	//mPendingInput = FVector::ZeroVector;

	//FVector	HorizontalVelocity = FVector(mVelocity.X, mVelocity.Y, 0.0);

	//// 입력받은 벡터에 최대 속도를 곱하여 입력받은 방향으로의 최대속도벡터를
	//// 만든다.
	//FVector	MaxInputVelocity = Input * mMaxSpeed;

	//// 가속도 적용
	//HorizontalVelocity = FMath::VInterpConstantTo(HorizontalVelocity,
	//	MaxInputVelocity, DeltaTime, mAcceleration);

	//mVelocity.Z += mGravityAccel * DeltaTime;

	//mVelocity.X = HorizontalVelocity.X;
	//mVelocity.Y = HorizontalVelocity.Y;

	//FVector	MoveDelta = mVelocity * DeltaTime;

	//// 업데이트할 컴포넌트의 현재 위치와 이동할 위치의 거리를 구하여
	//// 이 거리가 위에서 구해준 현재 이동해야 하는 양보다 작거나 같을 경우
	//// 목표지점을 지나가게 되는 것이다.
	//if (mPathArray.Num() > 0 &&
	//	mCurrentPathIndex < mPathArray.Num())
	//{
	//	float	TargetDist = FVector::Dist(CurrentLocation,
	//		mPathArray[mCurrentPathIndex]);

	//	// 이 상태는 타겟을 지나치게 될 것이다.
	//	if (TargetDist <= MoveDelta.Size())
	//	{
	//		// 이동 방향을 가지고 와서 타겟과의 거리만큼 이동할 수 있게
	//		// 벡터를 만든다.
	//		MoveDelta = MoveDelta.GetSafeNormal() * TargetDist;
	//		++mCurrentPathIndex;
	//	}
	//}

	//FHitResult	Hit;
	//// GetComponentRotation : Component의 월드 회전 정보
	//SafeMoveUpdatedComponent(MoveDelta,
	//	UpdatedComponent->GetComponentRotation(), true,
	//	Hit);

	//// 다른 물체와 부딪혔을 경우
	//if (Hit.IsValidBlockingHit())
	//{
	//	if (Hit.Normal.Z > 0.7f)
	//	{
	//		mVelocity.Z = 0.f;
	//	}

	//	SlideAlongSurface(MoveDelta, 1.f - Hit.Time, Hit.Normal,
	//		Hit);
	//}
}

void UAIMovement::ApplyControlInputToVelocity(float DeltaTime)
{
	const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

	const float AnalogInputModifier = (ControlAcceleration.SizeSquared() > 0.f ? ControlAcceleration.Size() : 0.f);
	const float MaxPawnSpeed = GetMaxSpeed() * AnalogInputModifier;
	const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(MaxPawnSpeed);

	if (AnalogInputModifier > 0.f && !bExceedingMaxSpeed)
	{
		// Apply change in velocity direction
		if (Velocity.SizeSquared() > 0.f)
		{
			// Change direction faster than only using acceleration, but never increase velocity magnitude.
			const float TimeScale = FMath::Clamp(DeltaTime * mTurningBoost, 0.f, 1.f);
			Velocity = Velocity + (ControlAcceleration * Velocity.Size() - Velocity) * TimeScale;
		}
	}
	else
	{
		// Dampen velocity magnitude based on deceleration.
		if (Velocity.SizeSquared() > 0.f)
		{
			const FVector OldVelocity = Velocity;
			const float VelSize = FMath::Max(Velocity.Size() - FMath::Abs(mDeceleration) * DeltaTime, 0.f);
			Velocity = Velocity.GetSafeNormal() * VelSize;

			// Don't allow braking to lower us below max speed if we started above it.
			if (bExceedingMaxSpeed && Velocity.SizeSquared() < FMath::Square(MaxPawnSpeed))
			{
				Velocity = OldVelocity.GetSafeNormal() * MaxPawnSpeed;
			}
		}
	}

	// Apply acceleration and clamp velocity magnitude.
	const float NewMaxSpeed = (IsExceedingMaxSpeed(MaxPawnSpeed)) ? Velocity.Size() : MaxPawnSpeed;
	Velocity += ControlAcceleration * FMath::Abs(mAcceleration) * DeltaTime;
	Velocity = Velocity.GetClampedToMaxSize(NewMaxSpeed);

	ConsumeInputVector();
}

bool UAIMovement::LimitWorldBounds()
{
	AWorldSettings* WorldSettings = PawnOwner ? PawnOwner->GetWorldSettings() : NULL;
	if (!WorldSettings || !WorldSettings->AreWorldBoundsChecksEnabled() || !UpdatedComponent)
	{
		return false;
	}

	const FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	if (CurrentLocation.Z < WorldSettings->KillZ)
	{
		Velocity.Z = FMath::Min<FVector::FReal>(GetMaxSpeed(), WorldSettings->KillZ - CurrentLocation.Z + 2.0f);
		return true;
	}

	return false;
}

bool UAIMovement::ResolvePenetrationImpl(const FVector& Adjustment, const FHitResult& Hit, const FQuat& NewRotationQuat)
{
	mbPositionCorrected |= Super::ResolvePenetrationImpl(Adjustment, Hit, NewRotationQuat);
	return mbPositionCorrected;
}

void UAIMovement::ClearPath()
{
	mPathArray.RemoveAll([](FVector v) { return true; });
}

