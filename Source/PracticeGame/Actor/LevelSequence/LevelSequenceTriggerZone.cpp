// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSequenceTriggerZone.h"


// Sets default values
ALevelSequenceTriggerZone::ALevelSequenceTriggerZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mTriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));

	SetRootComponent(mTriggerZone);

	mTriggerZone->SetIsVisualizationComponent(true);
	mTriggerZone->SetCollisionProfileName(TEXT("CinematicTriggerZone"));
}

// Called when the game starts or when spawned
void ALevelSequenceTriggerZone::BeginPlay()
{
	Super::BeginPlay();
	

	mTriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ALevelSequenceTriggerZone::OnBeginOverlap);
	mTriggerZone->OnComponentEndOverlap.AddDynamic(this, &ALevelSequenceTriggerZone::OnEndOverlap);
}

// Called every frame
void ALevelSequenceTriggerZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelSequenceTriggerZone::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	UE_LOG(TestGame, Warning, TEXT("LevelSequenceTriggerZone :: Begin Overlap Called"));

	if (IsValid(mSequence))
	{
		if (!IsValid(mSequencePlayer))
		{
			ALevelSequenceActor* SequenceActor = nullptr;
			//재생할 시퀀스 플레이어가 없을 경우 생성하기
			mSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), mSequence, mPlaySetting, SequenceActor);
			mSequenceActor = SequenceActor;

			//시퀀스가 종료될 때 호출할 함수
			mSequencePlayer->OnFinished.AddDynamic(this,&ALevelSequenceTriggerZone::SequenceOnFinishPlay);

			mSequencePlayer->Play();
			UE_LOG(TestGame, Warning, TEXT("SequencePlayer :: OnOverlap :: Play Sequence"));
		}
		else
		{
			//시퀀스가 종료될 때 호출할 함수
			mSequencePlayer->OnFinished.AddDynamic(this, &ALevelSequenceTriggerZone::SequenceOnFinishPlay);

			mSequencePlayer->Play();
			UE_LOG(TestGame, Warning, TEXT("SequencePlayer :: OnOverlap :: Play Sequence (Triggered Again)"));
		}
	}
}

void ALevelSequenceTriggerZone::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	UE_LOG(TestGame, Warning, TEXT("LevelSequenceTriggerZone :: End Overlap Called"));

	if (mIsReversePlay)
	{
		if (mSequencePlayer)
		{
			mSequencePlayer->PlayReverse();
		}
	}
}

void ALevelSequenceTriggerZone::SequenceOnFinishPlay()
{

	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
		return;

	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (!Player)
		return;
	//플레이어 숨김 / 무브먼트 비활성화 / 룩앳입력 비활성화 / 다시 되돌리기 
	Player->SetActorHiddenInGame(false);
	Player->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	Player->EnableInput(PC);
}

//
//DisableInput() 또는 GetCharacterMovement()->DisableMovement()
//룩어라운드(마우스 회전 등) 비활성화	Look Input Disable	DisableInput() 또는 커스텀 분기
//플레이어 숨김	Player Hide	SetActorHiddenInGame(true) 또는 GetMesh()->SetVisibility(false)