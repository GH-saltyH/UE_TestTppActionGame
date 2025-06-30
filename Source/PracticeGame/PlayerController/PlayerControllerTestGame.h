// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"			//UInputMappingContext 와 멤버들을 사용하려면 필요하다
#include "InputAction.h"							//UInputAction 과 멤버들을 사용하려면 필요하다
#include "../UI/MainWidget.h"
#include "PlayerControllerTestGame.generated.h"

UCLASS(Blueprintable)
class PRACTICEGAME_API APlayerControllerTestGame : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerTestGame();

public:
	//매핑컨텍스트를 들고 있는다
	TObjectPtr<UInputMappingContext> mIMC;
	//입력액션 IA_Move 를 들고 있는다
	UInputAction* mIAMove = nullptr;
	//입력액션 IA_Jump 를 들고 있는다
	UInputAction* mIAJump = nullptr;
	//입력액션 IA_Attack 을 들고있는다
	UInputAction* mIAAttack = nullptr;
	//입력액션IA_CameraRotation 을 들고있는다
	UInputAction* mIACameraRotate = nullptr;
	//입력액션IA_Dash 를 들고있는다
	UInputAction* mIADash = nullptr;
	//입력액션IA_Sprint 를 들고있는다
	UInputAction* mIASprint = nullptr;
	//입력액션IA_IASpecial 를 들고있는다
	UInputAction* mIASpecial = nullptr;
	//입력액션IA_CharacterRotate 를 들고 있는다 
	UInputAction* mIACharacterRotate = nullptr;
	//입력액션IA_PreviewThanCast 를 들고 있는다 
	UInputAction* mIAPreviewThanCast = nullptr;
	//입력액션IA_CancelAcceptCast 를 들고 있는다 
	UInputAction* mIAAcceptCastZone = nullptr;
	//입력액션IA_CancelAcceptCast 를 들고 있는다 
	UInputAction* mIACancelCastZone = nullptr;
	//입력액션IA_CancelAcceptCast 를 들고 있는다 
	UInputAction* mIACleave = nullptr;
	//입력액션IA_PlayerInfoPanel 을 들고 있는다
	UInputAction* mIAPlayerInfoPanel = nullptr;
	//입력액션IA_SelectCharacter 을 들고 있는다
	UInputAction* mIASelectCharacter = nullptr;

	UInputAction* mIAPanelSkill = nullptr;
	UInputAction* mIAPanelInv = nullptr;


protected:
	FString mPlayerName;

	//메인 위젯을 들고 있게 한다
	TSubclassOf<UUserWidget>	mMainWidgetClass;
	TObjectPtr<UMainWidget>		mMainWidget;

private:
	class ICharacterRenderingInterface* mHoveredActor = nullptr;
	class ICharacterRenderingInterface* mCurrentSelectedActor = nullptr;
	TArray<TObjectPtr<UMaterialInstanceDynamic>>		mPostProcessMtrls;

public:
	void SetPlayerName(const FString& NewPlayerName)
	{
		mPlayerName = NewPlayerName;
	}

	const FString& GetPlayerName() const
	{
		return mPlayerName;
	}

	void GetPlayerName(FString& OutPlayerName) const
	{
		OutPlayerName = mPlayerName;
	}

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;

protected:
	//플레이어 컨트롤러가 커스텀 입력 할당을 설정할 수 있도록 해준다
	virtual void SetupInputComponent() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SelectNPC();
	void DeselectNPC();
	AActor* GetCurrentSelectedActor();
};
