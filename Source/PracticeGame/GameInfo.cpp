// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInfo.h"
DEFINE_LOG_CATEGORY(TestGame);

FRotator GetTargetRotator(const FVector& Target, const FVector& Current)
{
	return UKismetMathLibrary::FindLookAtRotation(Current, Target);
}

FRotator GetTargetRotatorYaw(FVector Target, FVector Current)
{
	Target.Z = 0.0;
	Current.Z = 0.0;

	return UKismetMathLibrary::FindLookAtRotation(Current, Target);
}
