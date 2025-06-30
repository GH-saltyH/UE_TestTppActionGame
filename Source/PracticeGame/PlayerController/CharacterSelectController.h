// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"			//UInputMappingContext 와 멤버들을 사용하려면 필요하다
#include "InputAction.h"							//UInputAction 과 멤버들을 사용하려면 필요하다
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
	//매핑컨텍스트를 들고 있는다
	TObjectPtr<UInputMappingContext> mIMC;
	//입력액션IA_Dash 를 들고있는다
	UInputAction* mIASelectCharacter = nullptr;
	//입력액션IA_Sprint 를 들고있는다
	UInputAction* mIADeselectCharacter = nullptr;

private:
	TSubclassOf<UUserWidget>		mSelectWidgetClass;
	TObjectPtr<class UCharacterSelectWidget>	mSelectWidget;

private:
	//마우스 오버 이벤트를 제어하기 위한 멤버. 이전에 오버된 타깃을 등록하고 리셋 할 수 있도록 도와준다.
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
