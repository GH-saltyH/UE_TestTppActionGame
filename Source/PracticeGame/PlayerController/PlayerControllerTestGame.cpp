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
	//틱 설정
	PrimaryActorTick.bCanEverTick = true;

	//마우스 커서를 보이게 한다
	bShowMouseCursor = true;

	//메인 위젯을 가져온다
	static ConstructorHelpers::FClassFinder<UUserWidget> MainWidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Main.UI_Main_C'"));
	if (MainWidgetClass.Succeeded())
		mMainWidgetClass = MainWidgetClass.Class;

	//IMC 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputMappingContext>
		IMC(
			TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_TestGame.IMC_TestGame'")
		);
	//성공하면 주소를 받아온다
	if (IMC.Succeeded())
		mIMC = IMC.Object;

	//IA_Move 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAMove(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAMove.Succeeded())
		mIAMove = IAMove.Object;

	//IA_Jump 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAJump(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAJump.Succeeded())
		mIAJump = IAJump.Object;

	//IA_Attack 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAAttack(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAAttack.Succeeded())
		mIAAttack = IAAttack.Object;	
	
	//IA_CameraRotate 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACamRotate(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_CameraRotate.IA_CameraRotate'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IACamRotate.Succeeded())
		mIACameraRotate = IACamRotate.Object;

	//IA_Dash 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IADash(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Dash.IA_Dash'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IADash.Succeeded())
		mIADash = IADash.Object;

	//IA_Dash 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IASprint(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Sprint.IA_Sprint'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IASprint.Succeeded())
		mIASprint = IASprint.Object;

	//IA_Special 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IASpecial(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Special.IA_Special'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IASpecial.Succeeded())
		mIASpecial = IASpecial.Object;

	//IA_CharacterRotate 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACharacterRotate(
			TEXT("Script/EnhancedInput.InputAction'/Game/Input/IA_CharacterRotate.IA_CharacterRotate'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IACharacterRotate.Succeeded())
		mIACharacterRotate = IACharacterRotate.Object;

	//IA_PreviewThanCast 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAPreviewThanCast(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PreviewThanCast.IA_PreviewThanCast'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAPreviewThanCast.Succeeded())
		mIAPreviewThanCast = IAPreviewThanCast.Object;

	//IA_AcceptCastZone 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAAcceptCastZone(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_AcceptCastZone.IA_AcceptCastZone'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAAcceptCastZone.Succeeded())
		mIAAcceptCastZone = IAAcceptCastZone.Object;

	//IA_CancelCastZone 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACancelCastZone(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_CancelCastZone.IA_CancelCastZone'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IACancelCastZone.Succeeded())
		mIACancelCastZone = IACancelCastZone.Object;

	//IA_Cleave 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IACleave(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Cleave.IA_Cleave'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IACleave.Succeeded())
		mIACleave = IACleave.Object;

		//IA_Cleave 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAPlayerInfo(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PlayerInfoPanel.IA_PlayerInfoPanel'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAPlayerInfo.Succeeded())
		mIAPlayerInfoPanel = IAPlayerInfo.Object;

	//IA_Cleave 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IASelectCharacter(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_SelectCharacter.IA_SelectCharacter'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IASelectCharacter.Succeeded())
		mIASelectCharacter= IASelectCharacter.Object;

	//IA_PanelSkill 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAPanelSkill(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PanelSkill.IA_PanelSkill'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAPanelSkill.Succeeded())
		mIAPanelSkill = IAPanelSkill.Object;

	//IA_PanelInv 를 로드한다
	static ConstructorHelpers::
		FObjectFinder
		<UInputAction>
		IAPanelInv(
			TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_PanelInv.IA_PanelInv'")
		);
	//로드에 성공하면 주소를 받아온다
	if (IAPanelInv.Succeeded())
		mIAPanelInv = IAPanelInv.Object;
}

void APlayerControllerTestGame::BeginPlay()
{
	Super::BeginPlay();

	//메인 위젯을 화면에 생성한다
	mMainWidget = CreateWidget<UMainWidget>(GetWorld(), mMainWidgetClass);
	
	if(IsValid(mMainWidget))
		mMainWidget->AddToViewport();
	UE_LOG(TestGame, Warning, TEXT("MainWidget Added to viewport"));
	//인풋모드를  게임과 UI 로 설정한다 -> 생성자에 있으면 **this*  nullptr 을 유발한다
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	//월드에서 APostProcessVolume 스테틱클래스 엑터 하나를 찾아서 형변환하여 받아온다
	APostProcessVolume* PostProcess =
		Cast<APostProcessVolume>(UGameplayStatics::GetActorOfClass(GetWorld(), APostProcessVolume::StaticClass()));

	//UGameplayStatics::GetAllActorOfClass  -> 해당 클래스의 모든 엑터들을 받아온다

	if (IsValid(PostProcess))
	{
		FPostProcessSettings& Settings = PostProcess->Settings;
		//블렌더블을 돌면서 머티리얼을 받아오고 다이내믹 머티리얼로 만들어준다
		int32 Count = Settings.WeightedBlendables.Array.Num();

		for (int32 i = 0; i < Count; i++)
		{
			UMaterialInterface* OriginMtrl =
				Cast<UMaterialInterface>(Settings.WeightedBlendables.Array[i].Object);

			//머티리얼이 있으면 다이내믹 머티리얼로 교체
			if (IsValid(OriginMtrl))
			{
				UMaterialInstanceDynamic* DynamicMtrl =
					UMaterialInstanceDynamic::Create(OriginMtrl, this);

				//다이내믹 머티리얼로 교체
				Settings.WeightedBlendables.Array[i].Object = DynamicMtrl;

				//멤버변수에 들고있게 한다
				mPostProcessMtrls.Add(DynamicMtrl);
			}
		}
	}
}

void APlayerControllerTestGame::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	//케릭터 정보들을 가져온다 BeginPlay 그다음 Possess  가 된다

	//현재 폰을 형변환하여 맞는 클래스인지 확인한다
	ICharacterPublicInterface* Info = Cast<ICharacterPublicInterface>(aPawn);

	if (Info)
	{
		//케릭터 이름을 할당한다
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

	//마우스 포인터로부터 존재하는 3차원 물체와 충돌하여 대상정보를 획득한다
	FHitResult Hit;
	bool Pick = GetHitResultUnderCursor(ECollisionChannel::ECC_GameTraceChannel9, true, Hit);		//MouseTrace 채널로 설정하였다

	//충돌이 발생한 경우
	if (Pick)
	{
		//이 인터페이스를 상속하는 모든 엑터는 타깃 검출에 통과하게 될 것이다 
		//타깃의 클래스 유형이 많을수록 이런식의 처리가 개별 타깃 검사보다 유리하다
		ICharacterRenderingInterface* Target = Cast<ICharacterRenderingInterface>(Hit.GetActor());

		//기존 부딪힌 엑터정보가 선택된 엑터가 아닌경우 해제
		if (mHoveredActor && mHoveredActor != mCurrentSelectedActor)
		{	
			//기존의 엑터가 현재 엑터가 아닌경우
			if (mHoveredActor != Target)
			{
				mHoveredActor->EnableOutLine(false);
			}
		}
		
		//유효한 타깃과 충돌한 경우 새롭게 아웃라인적용
		if (Target)
		{
			Target->EnableOutLine(true);
		}

		mHoveredActor = Target;
	}

	//충돌이 발생하지 않은 경우  -> 기존의 아웃라인 제거 
	else
	{
		//기존의 아웃라인 존재하고 선택된 엑터가 아니면 제거
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

		//클릭했을 때 대상이 유효한 타깃인경우 아웃라인을 토글한다
		if (Target)
		{
			//기존 부딪힌 엑터정보가 있을경우
			if (mCurrentSelectedActor && mCurrentSelectedActor != Target)
			{
				mCurrentSelectedActor->EnableOutLine(false);		// 기존 아웃라인 해제

				Target->EnableOutLine(true);				// 새 타깃에 아웃라인 적용
				mCurrentSelectedActor = Target;					// 선택된 엑터로 갱신
				UE_LOG(TestGame, Display, TEXT("Clear and Replace SelectedActor"));
			}
			else
			{	//기존의 선택된 엑터가 없는 경우 새롭게 적용한다
				Target->EnableOutLine(true);				 // 아웃라인 적용
				mCurrentSelectedActor = Target;					// 선택된 엑터 저장
				UE_LOG(TestGame, Display, TEXT("Bind New SelectedActor Info"));
			}

			//위젯을 보이게 하고 내용을 업데이트한다
			//가져온다
			USelectedNPCInfoWidget* NPCPanel = CWidgetManager::GetInst()->FindWidget<USelectedNPCInfoWidget>(TEXT("SelectedNPCInfoWidget"));

			if (NPCPanel)
			{
				NPCPanel->SetVisibility(ESlateVisibility::Visible);
				//현재 대상 정보 넘김
				ICharacterPublicInterface* NPCInfo = Cast<ICharacterPublicInterface>(Target);
				if (NPCInfo)
				{
					NPCPanel->UpdateInfo(NPCInfo);
				}
			}

			//상인인 경우 처리한다
			ANPCMerchant* IsMerchantNPC = Cast<ANPCMerchant>(Target);
			if (IsMerchantNPC)
			{
				IsMerchantNPC->OnClicked();
			}
			//상인이 아닌경우 패널을 해제한다
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

		//기존의 선택된 엑터가 있으면 아웃라인을 제거하고 주소를 초기화한다
		if (mCurrentSelectedActor)
		{
			//기존 선택 엑터와 호버드 엑터가 같으면 호버드도 같이 삭제한다 (호버드 체크에서 예외처리되어서 제대로 제거 안되고 있기 때문)
			if (mCurrentSelectedActor == mHoveredActor)
			{
				UE_LOG(TestGame, Warning, TEXT("Deselect HoveredActor"));
				mHoveredActor->EnableOutLine(false);
				mHoveredActor = nullptr;
			}
			mCurrentSelectedActor->EnableOcclusion(false);		// 아웃라인 제거
			mCurrentSelectedActor = nullptr; // 선택 해제
			UE_LOG(TestGame, Warning, TEXT("Deselect Previous Selected Actor"));

		}
		//위젯을 안 보이게 한다
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
