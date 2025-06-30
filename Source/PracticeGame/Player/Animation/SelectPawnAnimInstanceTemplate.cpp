// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPawnAnimInstanceTemplate.h"

USelectPawnAnimInstanceTemplate::USelectPawnAnimInstanceTemplate()
{

}

void USelectPawnAnimInstanceTemplate::AnimNotify_SelectTransition()
{
	mAnim = ESelectAnim::Idle;
}
