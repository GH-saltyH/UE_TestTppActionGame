// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "MonsterController.h"
#include "MonsterData.h"
#include "MonsterTemplateAnimInstance.h"
#include "MonsterSpawnPoint.h"
#include "../../Player/PlayerCharacter.h"
#include "../../UI/MainGame/NPC/HPPlateWidget.h"
#include "../../UI/WidgetManager.h"
#include "../../UI/MainGame/NPC/SelectedNPCInfoWidget.h"

AMonster::AMonster()
{
	//자동빙의조건을  월드배치 및 생성으로 한다
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	//기본 빙의 콘트롤러를 MonsterController 로 설정한다
	AIControllerClass = AMonsterController::StaticClass();

	mMovement = CreateDefaultSubobject<UAIMovement>(TEXT("Movement"));
	mMovement->SetUpdatedComponent(RootComponent);

	mBody->SetCapsuleRadius(35.f);
	mBody->SetCollisionProfileName(TEXT("Monster"));
	
	mMesh->SetRelativeRotation(FRotator(0, -90, 0));
	
	//크기위치회전설정
	/*mBody->SetCapsuleHalfHeight(55.f);
	mBody->SetCapsuleRadius(55.f);

	mMesh->SetRelativeLocation(0, 0, 0);
	mMesh->SetRelativeRotation(0, 0, 0);*/

	//팀 ID 설정
	SetGenericTeamId(FGenericTeamId(TeamMonster));

	//자손클래스에서 다시 정의해야 한다 (생성자 호출 순서 상 어차피 자손들이 정한걸로 결정된다
	//애님인스턴스 (블루프린트 만들어서 할당해야 함
	//static ConstructorHelpers::FClassFinder<UAnimInstance>
	//	AnimAsset(TEXT("/Script/Engine.AnimBlueprint'/Game/Monster/ABPMinionDummy01.ABPMinionDummy01_C'"));

	////if (AnimAsset.Succeeded())
	//	mMesh->SetAnimInstanceClass(AnimAsset.Class);

	bUseControllerRotationYaw = true;
}

void AMonster::ChangeAnim(EMonsterDefaultAnim Anim)
{
	mMonsterAnim->ChangeAnim(Anim);
}


// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();

	//게임 시작 시 몬스터 애님인스턴스를 받아온다
	mMonsterAnim = Cast<UMonsterTemplateAnimInstance>(mMesh->GetAnimInstance());

	//몬스터의 Materiald을 DynamicMaterialInstance로 생성해준다
	int32	Count = mMesh->GetNumMaterials();

	//소유한 머티리얼들에 다이내믹 머티리얼  인스턴스를 생성한다
	for (int32 i = 0; i < Count; i++)
	{
		UMaterialInstanceDynamic* Mtid = mMesh->CreateDynamicMaterialInstance(i); 

		//특정 영역이 시간에 지남에따라 지워져야 하므로  마스크로 블렌드 모드를 바꾼다
		Mtid->BlendMode = EBlendMode::BLEND_Masked;
		mMaterials.Add(Mtid);
	}

	//HP 플레이트 초기화 (이름 과 생명력바)
/*UHPPlateWidget* PlateWidget = CWidgetManager::GetInst()->FindWidget<UHPPlateWidget>(TEXT("HPPlateWidget"));*/
	UHPPlateWidget* PlateWidget = Cast<UHPPlateWidget>(mWidgetComponent->GetWidget());
	if (PlateWidget)
	{
		PlateWidget->SetNPCName(mName);
		PlateWidget->SetHPBar(mHP, mHPMax);
	}
}
/*
* EEndPlayReason::Destroyed 엑터가 제거될 때 Destroy 함수 호출시
* EEndPlayReason::LeveTransition  다른 레벨로 전환되어 엑터가 언로드 될 때
* EEndPlayReason::EndPlayInEditor 에디터 모드에서 플레이를 중지할 때
* EEndPlayReason::RemovedFromWorld	액터가 월드에서 제거될 때 서브 ㄹ레벨 언로드 혹은 엑터 제거시
* EEndPlayReason::Quit 게임 종료시
*/
void AMonster::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);


	switch (EndPlayReason)
	{
	//
	case EEndPlayReason::Destroyed:
		if (mSpawnPoint)
			mSpawnPoint->ClearSpawnMonster();
		break;
	case EEndPlayReason::LevelTransition:
		break;
	case EEndPlayReason::EndPlayInEditor:
		break;
	case EEndPlayReason::RemovedFromWorld:
		break;
	case EEndPlayReason::Quit:
		break;
	}
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//번 지속 시간의 배율 (1.0f 보다 낮을 수록 느려짐)
	float SecondMultiplier = 1.333f;

	if (mIsBurn)
	{
		//설정한 시간에 따라 번 효과를 진행하게 한다
		mBurn -= 1.1f * DeltaTime * SecondMultiplier;

		if (mBurn <= -1.f)
		{
			//전부 탄 경우 물리 시뮬을 중단하고 엑터를 파괴한다
			mBody->SetCollisionProfileName(TEXT("Monster"));
			mMesh->SetSimulatePhysics(false);
			Destroy();
		}
		
		//Burn 값을 DynamicMaterialInstance 의 Parameter 에 넘겨준다
		int32	Count = mMaterials.Num();

		for (int32 i = 0; i < Count; i++)
		{
			mMaterials[i]->SetScalarParameterValue(TEXT("Erode"), mBurn);
		}
	}

	//피격 효과가 켜져있는 경우
	if (mHitFXEnable)
	{
		mHitTime += DeltaTime;

		if (mHitTime >= 0.1f)
		{
			mHitTime = 0.f;
			mHitFXEnable = false;

			int32	Count = mMaterials.Num();

			for (int32 i = 0; i < Count; ++i)
			{
				//
				mMaterials[i]->SetScalarParameterValue(TEXT("HitEnable"), 0.f);

			}
		}
	}
}


void AMonster::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const UMonsterData* MonsterData =
		GetDefault<UMonsterData>();

	if (MonsterData)
	{
		FMonsterTableInfo* TableInfo =
			MonsterData->FindMonsterInfo(mDataKey);

		if (TableInfo)
		{
			mName = TableInfo->Name;
			mAttack = TableInfo->Attack;
			mDefense = TableInfo->Defense;
			mHP = TableInfo->HP;
			mHPMax = TableInfo->HP;
			mMP = TableInfo->MP;
			mMPMax = TableInfo->MP;
			mAttackDistance = TableInfo->AttackDistance;
			mMoveSpeed = TableInfo->MoveSpeed;
			mDetectDistance = TableInfo->DetectDistance;
			mLevel = TableInfo->Level;
			mExp = TableInfo->Exp;
			mGold = TableInfo->Gold;

			mMovement->SetMaxSpeed(mMoveSpeed);

			UE_LOG(TestGame, Warning, TEXT("Monster Info"));
		}
	}
}

float AMonster::TakeDamage(float DamageAmount,
	FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageAmount = Super::TakeDamage(DamageAmount, DamageEvent,
		EventInstigator, DamageCauser);

	if (DamageAmount > 0.f)
	{
		mIsHit = true;	//생명력 플레이트 표시를 위한 플래그 설정
		float	Dmg = DamageAmount - mDefense;
		Dmg = Dmg < 1.f ? 1.f : Dmg;

		// 죽었을 경우
		if (Damage(Dmg))
		{
			
			ChangeAnim(EMonsterDefaultAnim::Death);

			//이동을 멈추고 AI 를 멈춘다
			AAIController* AI = GetController<AAIController>();

			AI->StopMovement();
			AI->BrainComponent->StopLogic(TEXT("Death"));

			//생명력 판정으로 죽었지만 여기에서는 아직 엑터가 사라진건 아니다
			// 죽는 모션의 마지막 노티파이가 호출될때까지는 객체가 남아있다.
			// 죽는 모션이 끝나는 동안 공격이 더 들어가지 않도록 충돌 프로파일을 처리한다
			mBody->SetCollisionProfileName("MonsterOnDeath");

			//번 FX 가 보이게 한다
			mIsBurn = true;

			//피격 FX 효과가 나오지 않게 한다
			mHitFXEnable = false;

			// 죽는 모션의 끝부분에 노티파이를 만들고 해당 노티파이 발생시 액터 제거.

			//행위자가 플레이어라면 자신의 경험치와 골드를 제공한다
			APlayerCharacter* Player = Cast<APlayerCharacter>(EventInstigator->GetPawn());
			if (Player)
			{
				UE_LOG(TestGame, Warning, TEXT("Player Gain %d Exp Points and %d Golds"), mExp, mGold);
				Player->OnGetExp(mExp);  // 이 함수는 xp 계산 및 레벨업 그리고 Exp 정보도 갱신한다
				Player->AddGold(mGold);
				
				//관련된 델리게이트 실행
				Player->GetQuestComponent()
					->mOnQuestTriggered.Broadcast(EQuestTaskType::KILL, this);
				
				//테스트용 -> 100% 확률로 테이블 내의 랜덤 아이템 1개 생성 
				Player->GenerateRandomInvItem();

				//플레이어 정보 패널이 열려있는 경우 정보 업데이트 
				UCharacterInfoWidget* CharInfoWidget = CWidgetManager::GetInst()->FindWidget<UCharacterInfoWidget>(TEXT("CharacterInfoWidget"));

				if (CharInfoWidget && CharInfoWidget->GetVisibility() == ESlateVisibility::Visible)
				{
					ICharacterPublicInterface* Info = Cast<ICharacterPublicInterface>(Player);

					if(Info)
						CharInfoWidget->UpdatePlayerInfo(Info);
				}
			}
		}
		//죽지 않았을 경우 -> 히트 모션처리 / 
		else
		{
			int32 Count = mMaterials.Num();

			for (int32 i = 0; i < Count; i++)
			{
				mMaterials[i]->SetScalarParameterValue(TEXT("HitEnable"), 1.f);
			}

			//피격 FX 가 나오게 한다
			mHitFXEnable = true;
			mMonsterAnim->Hit();
		}

		//FCT 생성 
		if (UMainWidget* MainWidget = CWidgetManager::GetInst()->FindWidget<UMainWidget>(TEXT("MainWidget")))
		{
			
			MainWidget->SpawnFloatingText(Dmg, this);
		}

		//HP Plate 업데이트하기
		UpdateHPBar();
		//선택된 NPC 가 있고 현재 엑터와 동일한 경우 NPCInfo 패널 정보를 업데이트 시도한다
		//선택된 엑터에 대한 주소는 APlayerControllerTestGame 내부에서 들고있기 때문에 이를 가져오려 시도한다
		APlayerControllerTestGame* MainGameController = Cast<APlayerControllerTestGame>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		UE_LOG(TestGame, Warning, TEXT("Monster TakeDamage : Try Find MainGameController"));
		if (MainGameController)
		{
			UE_LOG(TestGame, Warning, TEXT("Monster TakeDamage : MainGameController Found"));
			//컨트롤러내부 함수 : 현재 월드에 선택된 엑터 주소가 있고 몬스터인지 체크
			AMonster* SelectedActor = Cast<AMonster>(MainGameController->GetCurrentSelectedActor());
			//선택된 타깃이 있고 데미지를 입은 자신과 같은 경우 정보 업데이트
			if (SelectedActor && SelectedActor == this)
			{
				UE_LOG(TestGame, Warning, TEXT("Monster TakeDamage : Selected Actor equal to This"));
				//UI가 있는지 체크 업데이트 함수 내부에서 패널이 켜져있는지 검사하기 떄문에 그거까지는 안해도 된다.
				USelectedNPCInfoWidget* NPCWidget = CWidgetManager::GetInst()->FindWidget< USelectedNPCInfoWidget>(TEXT("SelectedNPCInfoWidget"));
				if (NPCWidget)
				{
					UE_LOG(TestGame, Warning, TEXT("Monster TakeDamage : NPCWidget Found"));
					//자신의 정보를 건네주기위하여 정보로 형변환
					ICharacterPublicInterface* Info = Cast<ICharacterPublicInterface>(this);
					if (Info)
						UE_LOG(TestGame, Warning, TEXT("Monster TakeDamage : NPCINFO -> UpdateInfo"));
						NPCWidget->UpdateInfo(Info);
				}
			}
		}
		else
			UE_LOG(TestGame, Warning, TEXT("Monster TakeDamage : MainGameController not found"));
	}

	return DamageAmount;
}

void AMonster::NormalAttack()
{
}

void AMonster::SpecialAttack()
{
}

void AMonster::RagDoll()
{
	mHitFXEnable = true;
	//래그돌이 시작되면 다시 플레이어와 충돌하도록 해준다
	//mMesh->SetCollisionProfileName(TEXT("Monster"));
	mMesh->SetSimulatePhysics(true);
}

void AMonster::UpdateHPBar()
{
	//HUD 에 사용한 마나를 반영한다
	//1. 싱글턴 매니저로부터 해당 HUD 위젯의 주소를 가져온다 
	/*UHPPlateWidget* PlateWidget = CWidgetManager::GetInst()->FindWidget<UHPPlateWidget>(TEXT("HPPlateWidget"));*/
	UHPPlateWidget* PlateWidget = Cast<UHPPlateWidget>(mWidgetComponent->GetWidget());
	if (PlateWidget)
	{
		PlateWidget->SetHPBar(mHP, mHPMax);
	}
}
