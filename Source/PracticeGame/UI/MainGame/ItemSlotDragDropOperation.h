// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UIInfo.h"
#include "../../GameInfo.h"
#include "Blueprint/DragDropOperation.h"
#include "ItemSlotDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UItemSlotDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	int32		mPrevSlotIndex = -1;
	
	
};
