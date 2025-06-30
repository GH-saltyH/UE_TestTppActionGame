// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerTestGame.h"
#include "../Interface/CharacterRenderingInterface.h"
#include "../NPC/Merchant/NPCMerchant.h"
#include "../Player/PlayerCharacter.h"
#include "../Interface/CharacterPublicInterface.h"
#include "../UI/WidgetManager.h"
#include "../UI/MainGame/NPC/SelectedNPCInfoWidget.h"

APlayerControllerTestGame::APlayerControllerTestGame()
{
	//ƽ ����
	PrimaryActorTick.bCanEverTick = true;

	//���콺 Ŀ���� ���̰� �Ѵ�
	bShowMouseCursor = true;

	//���� ������ �����´�
	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Main.UI_Main_C'"));
	if (MainWidgetClass.Succeeded())
		mMainWidgetClass = MainWidgetClass.Class;

	//IMC �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputMappingContext>
		IMC(
			TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_TestGame.IMC_TestGame'")
		);
	//�����ϸ� �ּҸ� �޾ƿ´�
	if (IMC.Succeeded())
		mIMC = IMC.Object;

	//IA_Move �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAMove(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAMove.Succeeded())
		mIAMove = IAMove.Object;

	//IA_Jump �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAJump(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAJump.Succeeded())
		mIAJump = IAJump.Object;

	//IA_Attack �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAAttack(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAAttack.Succeeded())
		mIAAttack = IAAttack.Object;	
	
	//IA_CameraRotate �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACamRotate(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_CameraRotate.IA_CameraRotate'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IACamRotate.Succeeded())
		mIACameraRotate = IACamRotate.Object;

	//IA_Dash �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IADash(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Dash.IA_Dash'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IADash.Succeeded())
		mIADash = IADash.Object;

	//IA_Dash �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IASprint(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Sprint.IA_Sprint'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IASprint.Succeeded())
		mIASprint = IASprint.Object;

	//IA_Special �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IASpecial(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Special.IA_Special'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IASpecial.Succeeded())
		mIASpecial = IASpecial.Object;

	//IA_CharacterRotate �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACharacterRotate(
			TEXT("Script/EnhancedInput.InputAction'/Game/Input/IA_CharacterRotate.IA_CharacterRotate'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IACharacterRotate.Succeeded())
		mIACharacterRotate = IACharacterRotate.Object;

	//IA_PreviewThanCast �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAPreviewThanCast(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PreviewThanCast.IA_PreviewThanCast'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAPreviewThanCast.Succeeded())
		mIAPreviewThanCast = IAPreviewThanCast.Object;

	//IA_AcceptCastZone �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAAcceptCastZone(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_AcceptCastZone.IA_AcceptCastZone'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAAcceptCastZone.Succeeded())
		mIAAcceptCastZone = IAAcceptCastZone.Object;

	//IA_CancelCastZone �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACancelCastZone(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_CancelCastZone.IA_CancelCastZone'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IACancelCastZone.Succeeded())
		mIACancelCastZone = IACancelCastZone.Object;

	//IA_Cleave �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACleave(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Cleave.IA_Cleave'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IACleave.Succeeded())
		mIACleave = IACleave.Object;

		//IA_Cleave �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAPlayerInfo(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerInfoPanel.IA_PlayerInfoPanel'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAPlayerInfo.Succeeded())
		mIAPlayerInfoPanel = IAPlayerInfo.Object;

	//IA_Cleave �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IASelectCharacter(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SelectCharacter.IA_SelectCharacter'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IASelectCharacter.Succeeded())
		mIASelectCharacter= IASelectCharacter.Object;

	//IA_PanelSkill �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAPanelSkill(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PanelSkill.IA_PanelSkill'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAPanelSkill.Succeeded())
		mIAPanelSkill = IAPanelSkill.Object;

	//IA_PanelInv �� �ε��Ѵ�
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAPanelInv(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PanelInv.IA_PanelInv'")
		);
	//�ε忡 �����ϸ� �ּҸ� �޾ƿ´�
	if (IAPanelInv.Succeeded())
		mIAPanelInv = IAPanelInv.Object;
}

void APlayerControllerTestGame::BeginPlay()
{
	Super::BeginPlay();

	//���� ������ ȭ�鿡 �����Ѵ�
	mMainWidget = CreateWidget<UMainWidget>(GetWorld(), mMainWidgetClass);
	
	if(IsValid(mMainWidget))
		mMainWidget->AddToViewport();
	UE_LOG(TestGame, Warning, TEXT("MainWidget Added to viewport"));
	//��ǲ��带  ���Ӱ� UI �� �����Ѵ� -> �����ڿ� ������ **this*  nullptr �� �����Ѵ�
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	//���忡�� APostProcessVolume ����ƽŬ���� ���� �ϳ��� ã�Ƽ� ����ȯ�Ͽ� �޾ƿ´�
	APostProcessVolume* PostProcess =
		Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));

	//UGameplayStatics::GetAllActorOfClass  -> �ش� Ŭ������ ��� ���͵��� �޾ƿ´�

	if (IsValid(PostProcess))
	{
		FPostProcessSettings& Settings = PostProcess->Settings;
		//�������� ���鼭 ��Ƽ������ �޾ƿ��� ���̳��� ��Ƽ����� ������ش�
		int32 Count = Settings.WeightedBlendables.Array.Num();

		for (int32 i = 0; i < Count; i++)
		{
			UMaterialInterface* OriginMtrl =
				Cast<UMaterialInterface>(Settings.WeightedBlendables.Array[i].Object);

			//��Ƽ������ ������ ���̳��� ��Ƽ����� ��ü
			if (IsValid(OriginMtrl))
			{
				UMaterialInstanceDynamic* DynamicMtrl =
					UMaterialInstanceDynamic::Create(OriginMtrl, this);

				//���̳��� ��Ƽ����� ��ü
				Settings.WeightedBlendables.Array[i].Object = DynamicMtrl;

				//��������� ����ְ� �Ѵ�
				mPostProcessMtrls.Add(DynamicMtrl);
			}
		}
	}
}

void APlayerControllerTestGame::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	//�ɸ��� �������� �����´� BeginPlay �״��� Possess  �� �ȴ�

	//���� ���� ����ȯ�Ͽ� �´� Ŭ�������� Ȯ���Ѵ�
	ICharacterPublicInterface* Info = Cast<ICharacterPublicInterface>(aPawn);

	if (Info)
	{
		//�ɸ��� �̸��� �Ҵ��Ѵ�
		Info->SetName(mPlayerName);
		UE_LOG(TestGame, Warning, TEXT("OnPossess : character name set to %d"), *mPlayerName);
	}

}

void APlayerControllerTestGame::OnUnPossess()
{
	Super::OnUnPossess();

}

void APlayerControllerTestGame::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void APlayerControllerTestGame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//���콺 �����ͷκ��� �����ϴ� 3���� ��ü�� �浹�Ͽ� ��������� ȹ���Ѵ�
	FHitResult Hit;
	bool Pick = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, true, Hit);		//MouseTrace ä�η� �����Ͽ���

	//�浹�� �߻��� ���
	if (Pick)
	{
		//�� �������̽��� ����ϴ� ��� ���ʹ� Ÿ�� ���⿡ ����ϰ� �� ���̴� 
		//Ÿ���� Ŭ���� ������ �������� �̷����� ó���� ���� Ÿ�� �˻纸�� �����ϴ�
		ICharacterRenderingInterface* Target = Cast<ICharacterRenderingInterface>(Hit.GetActor());

		//���� �ε��� ���������� ���õ� ���Ͱ� �ƴѰ�� ����
		if (mHoveredActor && mHoveredActor != mCurrentSelectedActor)
		{	
			//������ ���Ͱ� ���� ���Ͱ� �ƴѰ��
			if (mHoveredActor != Target)
			{
				mHoveredActor->EnableOutLine(false);
			}
		}
		
		//��ȿ�� Ÿ��� �浹�� ��� ���Ӱ� �ƿ���������
		if (Target)
		{
			Target->EnableOutLine(true);
		}

		mHoveredActor = Target;
	}

	//�浹�� �߻����� ���� ���  -> ������ �ƿ����� ���� 
	else
	{
		//������ �ƿ����� �����ϰ� ���õ� ���Ͱ� �ƴϸ� ����
		if (mHoveredActor && mHoveredActor != mCurrentSelectedActor)
		{
			mHoveredActor->EnableOutLine(false);
			mHoveredActor = nullptr;
		}
	}
}

void APlayerControllerTestGame::SelectNPC()
{
	UE_LOG(TestGame, Display, TEXT("A NPC has Selected"));
	FHitResult Hit;
	bool IsHit = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, false, Hit);

	if (IsHit)
	{
		ANPC* Target = Cast<ANPC>(Hit.GetActor());

		//Ŭ������ �� ����� ��ȿ�� Ÿ���ΰ�� �ƿ������� ����Ѵ�
		if (Target)
		{
			//���� �ε��� ���������� �������
			if (mCurrentSelectedActor && mCurrentSelectedActor != Target)
			{
				mCurrentSelectedActor->EnableOutLine(false);		// ���� �ƿ����� ����

				Target->EnableOutLine(true);				// �� Ÿ�꿡 �ƿ����� ����
				mCurrentSelectedActor = Target;					// ���õ� ���ͷ� ����
				UE_LOG(TestGame, Display, TEXT("Clear and Replace SelectedActor"));
			}
			else
			{	//������ ���õ� ���Ͱ� ���� ��� ���Ӱ� �����Ѵ�
				Target->EnableOutLine(true);				 // �ƿ����� ����
				mCurrentSelectedActor = Target;					// ���õ� ���� ����
				UE_LOG(TestGame, Display, TEXT("Bind New SelectedActor Info"));
			}

			//������ ���̰� �ϰ� ������ ������Ʈ�Ѵ�
			//�����´�
			USelectedNPCInfoWidget* NPCPanel = CWidgetManager::GetInst()->FindWidget<USelectedNPCInfoWidget>(TEXT("SelectedNPCInfoWidget"));

			if (NPCPanel)
			{
				NPCPanel->SetVisibility(ESlateVisibility::Visible);
				//���� ��� ���� �ѱ�
				ICharacterPublicInterface* NPCInfo = Cast<ICharacterPublicInterface>(Target);
				if (NPCInfo)
				{
					NPCPanel->UpdateInfo(NPCInfo);
				}
			}

			//������ ��� ó���Ѵ�
			ANPCMerchant* IsMerchantNPC = Cast<ANPCMerchant>(Target);
			if (IsMerchantNPC)
			{
				IsMerchantNPC->OnClicked();
			}
			//������ �ƴѰ�� �г��� �����Ѵ�
			else
			{
				UMainWidget* GetMerchant = CWidgetManager::GetInst()->FindWidget<UMainWidget>(TEXT("MainWidget"));
				if (GetMerchant)
				{
					GetMerchant->CloseTradePanel();
				}
			}
		}
	}
}

void APlayerControllerTestGame::DeselectNPC()
{

		//������ ���õ� ���Ͱ� ������ �ƿ������� �����ϰ� �ּҸ� �ʱ�ȭ�Ѵ�
		if (mCurrentSelectedActor)
		{
			//���� ���� ���Ϳ� ȣ���� ���Ͱ� ������ ȣ���嵵 ���� �����Ѵ� (ȣ���� üũ���� ����ó���Ǿ ����� ���� �ȵǰ� �ֱ� ����)
			if (mCurrentSelectedActor == mHoveredActor)
			{
				UE_LOG(TestGame, Warning, TEXT("Deselect HoveredActor"));
				mHoveredActor->EnableOutLine(false);
				mHoveredActor = nullptr;
			}
			mCurrentSelectedActor->EnableOcclusion(false);		// �ƿ����� ����
			mCurrentSelectedActor = nullptr; // ���� ����
			UE_LOG(TestGame, Warning, TEXT("Deselect Previous Selected Actor"));

		}
		//������ �� ���̰� �Ѵ�
		USelectedNPCInfoWidget* NPCPanel = CWidgetManager::GetInst()->FindWidget<USelectedNPCInfoWidget>(TEXT("SelectedNPCInfoWidget"));

		if (NPCPanel)
		{
			NPCPanel->SetVisibility(ESlateVisibility::Collapsed);
		}
}

AActor* APlayerControllerTestGame::GetCurrentSelectedActor()
{
	if(!mCurrentSelectedActor)
		return nullptr;

	AActor* SelectedActor = Cast<AActor>(mCurrentSelectedActor);
	if (SelectedActor)
		return SelectedActor;
	else
		return nullptr;
}
