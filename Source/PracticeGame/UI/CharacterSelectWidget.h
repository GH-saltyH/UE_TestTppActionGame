// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UserWidgetBase.h"
#include "SelectInfoWidget.h"
#include "CharacterSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UCharacterSelectWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	UCharacterSelectWidget(const FObjectInitializer& ObjectInitializer);

protected:
	TObjectPtr<UButton>		mStartButton;
	TObjectPtr<UButton>		mExitButton;

	TObjectPtr<USelectInfoWidget>	mInfoWidget;
	//이름 입력창 요소를 받을 위치
	TObjectPtr<UEditableTextBox>	mInputName;
	//선택한 직업을 가지고 있게 한다
	EPlayerJob			mSelectedJob;;

	

protected:
	virtual void NativeOnInitialized();

	//UFUNCTION()
	//void OnClickStartButton();
	//UFUNCTION()
	//void OnHoveredStartButton();
	//UFUNCTION()
	//void UnHoveredStartButton();

	//UFUNCTION()
	//void OnClickExitButton();
	//UFUNCTION()
	//void OnHoveredExitButton();
	//UFUNCTION()
	//void UnHoveredExitButton();
public:
	void EnableStartButton(bool IsEnable);
	void EnableInfoWidget(bool IsEnable);
	void SetCharacterImage(UMaterialInterface* Mtrl);
	void SetCharacterInfo(class ICharacterPublicInterface* Info);
	void SetSelectedJob(EPlayerJob Job)
	{
		mSelectedJob = Job;
	}

protected:
	UFUNCTION()
	void StartButtonOnClick();
	UFUNCTION()
	void ExitButtonOnClick();
};
