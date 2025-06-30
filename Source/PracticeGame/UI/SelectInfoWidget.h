// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UserWidgetBase.h"
#include "SelectInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API USelectInfoWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	USelectInfoWidget(const FObjectInitializer& ObjectInitializer);

public:
	TObjectPtr<UTextBlock>	mJob;
	TObjectPtr<UImage>			mCharacterImage;
	TObjectPtr<UTextBlock>	mCharacterName;
	TObjectPtr<UTextBlock>	mAttack;
	TObjectPtr<UTextBlock>	mDefense;
	TObjectPtr<UTextBlock>	mHP;
	TObjectPtr<UTextBlock>	mMP;

protected:
	virtual void NativeOnInitialized();

public:
	void SetCharacterImage(UMaterialInterface* Mtrl);
	void SetCharacterInfo(class ICharacterPublicInterface* Info);

};
