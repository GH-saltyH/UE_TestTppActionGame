// Fill out your copyright notice in the Description page of Project Settings.


#include "AIMovement.h"
#include "NavigationSystem.h"		//��ã�� ����� ����ϱ� ���� ����
#include "NavigationPath.h"			//��ã�� ����� ����ϱ� ���� ����
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"		//EPathFollowingStatus ���� ����ϱ� ���� ����

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
		case EPathFollowingStatus::Idle:		//�̵��� �ƴ� �� ó��
			Move(DeltaTime);
			break;
		case EPathFollowingStatus::Waiting:		//��ã�� ��
			break;
		case EPathFollowingStatus::Paused:		//�̵� �Ͻ� ����.
			break;
		case EPathFollowingStatus::Moving:		//�̵���
			Move(DeltaTime);
			mPendingInput = FVector::ZeroVector;
			break;
		}
	}

	return;

	//// ������Ʈ�� ���� �߹� ��ġ�� ���´�.
	//FVector	CurrentLocation = UpdatedComponent->GetComponentLocation();

	//UCapsuleComponent* Capsule =
	//	Cast<UCapsuleComponent>(UpdatedComponent);

	//// �߹� ��ġ�� ���Ѵ�.
	//if (IsValid(Capsule))
	//{
	//	CurrentLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
	//}

	//if (mPathArray.Num() > 0 &&
	//	mCurrentPathIndex < mPathArray.Num())
	//{
	//	FollowPath(CurrentLocation, DeltaTime);
	//}

	//// GetClampedToMaxSize �Լ��� ���ڷ� ���� ũ�⸦ �ʰ��ϴ�
	//// ��� ������ �ִ� ũ�⿡ ���� ũ�⸸ ���̰� ������ ������ ���ο�
	//// ���͸� ��ȯ�Ѵ�.
	//FVector	Input = mPendingInput.GetClampedToMaxSize(1.0);

	//mPendingInput = FVector::ZeroVector;

	//FVector	HorizontalVelocity = FVector(mVelocity.X, mVelocity.Y, 0.0);

	//// �Է¹��� ���Ϳ� �ִ� �ӵ��� ���Ͽ� �Է¹��� ���������� �ִ�ӵ����͸�
	//// �����.
	//FVector	MaxInputVelocity = Input * mMaxSpeed;

	//// ���ӵ� ����
	//HorizontalVelocity = FMath::VInterpConstantTo(HorizontalVelocity,
	//	MaxInputVelocity, DeltaTime, mAcceleration);

	//mVelocity.Z += mGravityAccel * DeltaTime;

	//mVelocity.X = HorizontalVelocity.X;
	//mVelocity.Y = HorizontalVelocity.Y;

	//FVector	MoveDelta = mVelocity * DeltaTime;

	//// ������Ʈ�� ������Ʈ�� ���� ��ġ�� �̵��� ��ġ�� �Ÿ��� ���Ͽ�
	//// �� �Ÿ��� ������ ������ ���� �̵��ؾ� �ϴ� �纸�� �۰ų� ���� ���
	//// ��ǥ������ �������� �Ǵ� ���̴�.
	//if (mPathArray.Num() > 0 &&
	//	mCurrentPathIndex < mPathArray.Num())
	//{
	//	float	TargetDist = FVector::Dist(CurrentLocation,
	//		mPathArray[mCurrentPathIndex]);

	//	// �� ���´� Ÿ���� ����ġ�� �� ���̴�.
	//	if (TargetDist <= MoveDelta.Size())
	//	{
	//		// �̵� ������ ������ �ͼ� Ÿ�ٰ��� �Ÿ���ŭ �̵��� �� �ְ�
	//		// ���͸� �����.
	//		MoveDelta = MoveDelta.GetSafeNormal() * TargetDist;
	//		++mCurrentPathIndex;
	//	}
	//}

	//FHitResult	Hit;
	//// GetComponentRotation : Component�� ���� ȸ�� ����
	//// MoveDelta ��ŭ �̵��ϰ� 
	//SafeMoveUpdatedComponent(MoveDelta,
	//	UpdatedComponent->GetComponentRotation(), true,
	//	Hit);

	//// �ٸ� ��ü�� �ε����� ���
	//if (Hit.IsValidBlockingHit())
	//{
	//	//�ε��� ���� �ٶ󺸴� ������ ���ʹ����� ��� �Ʒ������� �ε����ٰ� �Ǵ��Ͽ� 
	//	//�߷¿� ���� ������� �ӵ��� 0���� �ʱ�ȭ�Ѵ�
	//	if (Hit.Normal.Z > 0.7f)
	//	{
	//		mVelocity.Z = 0.f;
	//	}

	//	//��ü�� �ε����� ������ �ε��� 
	//	//��ü�� �ٶ󺸴� ������ �̿��Ͽ� �̲������� �̵��ϵ��� ó���Ѵ�
	//	SlideAlongSurface(MoveDelta, 1.f - Hit.Time, Hit.Normal,
	//		Hit);
	//}
}

void UAIMovement::FindPathToTarget()
{
	// ������̼� �����ڸ� ���´�.
	UNavigationSystemV1* NavSystem =
		FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());

	// ������̼� �ý����� �ְ� �� �����Ʈ ������Ʈ�� ������Ʈ�� ������Ʈ��
	// ���� ���
	if (NavSystem && UpdatedComponent)
	{
		FNavLocation	ProjectLocation;

		if (NavSystem->ProjectPointToNavigation(mTargetLocation,
			ProjectLocation))
		{
			// ���� ������Ʈ�� ��ġ�� ���´�.
			FVector	CurrentLocation = UpdatedComponent->GetComponentLocation();

			UCapsuleComponent* Capsule =
				Cast<UCapsuleComponent>(UpdatedComponent);

			// �߹� ��ġ�� ���Ѵ�.
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
				//0�� �ε����� �̵��� ���� ���� �����̱� ������ ���� �̵��� �̷�� ���� �ϴ� 
				//1�� �ε��� ���� �����ϰ� ó���Ѵ�
				mCurrentPathIndex = 1;
			}
		}
	}
}

void UAIMovement::FollowPath(const FVector& CurrentLocation, float DeltaTime)
{
	// ������ ���� ã�ư��ٴ� �ǹ��̴�.
	if (mCurrentPathIndex >= mPathArray.Num())
	{
		mPathArray.Empty();
		return;
	}

	// �̵��� ������ ���´�.
	FVector	TargetPoint = mPathArray[mCurrentPathIndex];

	//TargetPoint.Z = CurrentLocation.Z;
	// ��θ� ������ �� �߰��� ������̼� �޽��� ���̸�ŭ�� �����Ѵ�
	TargetPoint.Z -= 10.f;

	// �̵� ������ ���Ѵ�.
	FVector	Dir = TargetPoint - CurrentLocation;

	// ũ�Ⱑ 1�� ���ͷ� �����Ѵ�.
	Dir = Dir.GetSafeNormal();

	mPendingInput += Dir;

	// ���� �����ӿ� �̵� �������� �̵��� ���� ���Ѵ�.
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
	//// ������Ʈ�� ���� �߹� ��ġ�� ���´�.
	//FVector	CurrentLocation = UpdatedComponent->GetComponentLocation();

	//UCapsuleComponent* Capsule =
	//	Cast<UCapsuleComponent>(UpdatedComponent);

	//// �߹� ��ġ�� ���Ѵ�.
	//if (IsValid(Capsule))
	//{
	//	CurrentLocation.Z -= Capsule->GetScaledCapsuleHalfHeight();
	//}

	//if (mPathArray.Num() > 0 &&
	//	mCurrentPathIndex < mPathArray.Num())
	//{
	//	FollowPath(CurrentLocation, DeltaTime);
	//}

	//// GetClampedToMaxSize �Լ��� ���ڷ� ���� ũ�⸦ �ʰ��ϴ�
	//// ��� ������ �ִ� ũ�⿡ ���� ũ�⸸ ���̰� ������ ������ ���ο�
	//// ���͸� ��ȯ�Ѵ�.
	//FVector	Input = mPendingInput.GetClampedToMaxSize(1.0);

	//mPendingInput = FVector::ZeroVector;

	//FVector	HorizontalVelocity = FVector(mVelocity.X, mVelocity.Y, 0.0);

	//// �Է¹��� ���Ϳ� �ִ� �ӵ��� ���Ͽ� �Է¹��� ���������� �ִ�ӵ����͸�
	//// �����.
	//FVector	MaxInputVelocity = Input * mMaxSpeed;

	//// ���ӵ� ����
	//HorizontalVelocity = FMath::VInterpConstantTo(HorizontalVelocity,
	//	MaxInputVelocity, DeltaTime, mAcceleration);

	//mVelocity.Z += mGravityAccel * DeltaTime;

	//mVelocity.X = HorizontalVelocity.X;
	//mVelocity.Y = HorizontalVelocity.Y;

	//FVector	MoveDelta = mVelocity * DeltaTime;

	//// ������Ʈ�� ������Ʈ�� ���� ��ġ�� �̵��� ��ġ�� �Ÿ��� ���Ͽ�
	//// �� �Ÿ��� ������ ������ ���� �̵��ؾ� �ϴ� �纸�� �۰ų� ���� ���
	//// ��ǥ������ �������� �Ǵ� ���̴�.
	//if (mPathArray.Num() > 0 &&
	//	mCurrentPathIndex < mPathArray.Num())
	//{
	//	float	TargetDist = FVector::Dist(CurrentLocation,
	//		mPathArray[mCurrentPathIndex]);

	//	// �� ���´� Ÿ���� ����ġ�� �� ���̴�.
	//	if (TargetDist <= MoveDelta.Size())
	//	{
	//		// �̵� ������ ������ �ͼ� Ÿ�ٰ��� �Ÿ���ŭ �̵��� �� �ְ�
	//		// ���͸� �����.
	//		MoveDelta = MoveDelta.GetSafeNormal() * TargetDist;
	//		++mCurrentPathIndex;
	//	}
	//}

	//FHitResult	Hit;
	//// GetComponentRotation : Component�� ���� ȸ�� ����
	//SafeMoveUpdatedComponent(MoveDelta,
	//	UpdatedComponent->GetComponentRotation(), true,
	//	Hit);

	//// �ٸ� ��ü�� �ε����� ���
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

