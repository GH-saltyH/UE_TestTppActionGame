// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"			//UInputMappingContext �� ������� ����Ϸ��� �ʿ��ϴ�
#include "InputAction.h"							//UInputAction �� ������� ����Ϸ��� �ʿ��ϴ�
#include "CharacterSelectController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API ACharacterSelectController : public APlayerController
{
	GENERATED_BODY()
public:
	ACharacterSelectController();
	
protected:
	//�������ؽ�Ʈ�� ��� �ִ´�
	TObjectPtr<UInputMappingContext> mIMC;
	//�Է¾׼�IA_Dash �� ����ִ´�
	UInputAction* mIASelectCharacter = nullptr;
	//�Է¾׼�IA_Sprint �� ����ִ´�
	UInputAction* mIADeselectCharacter = nullptr;

private:
	TSubclassOf<UUserWidget>		mSelectWidgetClass;
	TObjectPtr<class UCharacterSelectWidget>	mSelectWidget;

private:
	//���콺 ���� �̺�Ʈ�� �����ϱ� ���� ���. ������ ������ Ÿ���� ����ϰ� ���� �� �� �ֵ��� �����ش�.
	class ASelectPawn* mHoveredActor = nullptr;

protected:
	class ASelectPawn* mCurrentSelectedActor = nullptr;

protected:
	class APlayerCharacter* mSelectedCharacterClass = nullptr;

protected:
	virtual void BeginPlay() override;

protected:
	void SetSelectionCamera();
	void SelectActor();
	void DeselectActor();
public:
	virtual void Tick(float DeltaSeconds);

protected:
	virtual void SetupInputComponent();
	
protected:
	void MoveCameraToSelectedActor(AActor* SelectedActor);
	void MoveCameraToDefault();

};
