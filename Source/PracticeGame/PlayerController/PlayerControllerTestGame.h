// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"			//UInputMappingContext �� ������� ����Ϸ��� �ʿ��ϴ�
#include "InputAction.h"							//UInputAction �� ������� ����Ϸ��� �ʿ��ϴ�
#include "../UI/MainWidget.h"
#include "PlayerControllerTestGame.generated.h"

UCLASS(Blueprintable)
class PRACTICEGAME_API APlayerControllerTestGame : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerControllerTestGame();

public:
	//�������ؽ�Ʈ�� ��� �ִ´�
	TObjectPtr<UInputMappingContext> mIMC;
	//�Է¾׼� IA_Move �� ��� �ִ´�
	UInputAction* mIAMove = nullptr;
	//�Է¾׼� IA_Jump �� ��� �ִ´�
	UInputAction* mIAJump = nullptr;
	//�Է¾׼� IA_Attack �� ����ִ´�
	UInputAction* mIAAttack = nullptr;
	//�Է¾׼�IA_CameraRotation �� ����ִ´�
	UInputAction* mIACameraRotate = nullptr;
	//�Է¾׼�IA_Dash �� ����ִ´�
	UInputAction* mIADash = nullptr;
	//�Է¾׼�IA_Sprint �� ����ִ´�
	UInputAction* mIASprint = nullptr;
	//�Է¾׼�IA_IASpecial �� ����ִ´�
	UInputAction* mIASpecial = nullptr;
	//�Է¾׼�IA_CharacterRotate �� ��� �ִ´� 
	UInputAction* mIACharacterRotate = nullptr;
	//�Է¾׼�IA_PreviewThanCast �� ��� �ִ´� 
	UInputAction* mIAPreviewThanCast = nullptr;
	//�Է¾׼�IA_CancelAcceptCast �� ��� �ִ´� 
	UInputAction* mIAAcceptCastZone = nullptr;
	//�Է¾׼�IA_CancelAcceptCast �� ��� �ִ´� 
	UInputAction* mIACancelCastZone = nullptr;
	//�Է¾׼�IA_CancelAcceptCast �� ��� �ִ´� 
	UInputAction* mIACleave = nullptr;
	//�Է¾׼�IA_PlayerInfoPanel �� ��� �ִ´�
	UInputAction* mIAPlayerInfoPanel = nullptr;
	//�Է¾׼�IA_SelectCharacter �� ��� �ִ´�
	UInputAction* mIASelectCharacter = nullptr;

	UInputAction* mIAPanelSkill = nullptr;
	UInputAction* mIAPanelInv = nullptr;


protected:
	FString mPlayerName;

	//���� ������ ��� �ְ� �Ѵ�
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
	//�÷��̾� ��Ʈ�ѷ��� Ŀ���� �Է� �Ҵ��� ������ �� �ֵ��� ���ش�
	virtual void SetupInputComponent() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SelectNPC();
	void DeselectNPC();
	AActor* GetCurrentSelectedActor();
};
