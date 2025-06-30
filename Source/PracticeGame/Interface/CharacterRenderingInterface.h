// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/Interface.h"
#include "CharacterRenderingInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterRenderingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PRACTICEGAME_API ICharacterRenderingInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void EnableOutLine(bool Enable);
	virtual void EnableOcclusion(bool Enable);
};
