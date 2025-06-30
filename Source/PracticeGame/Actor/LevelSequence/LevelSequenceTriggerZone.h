// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "LevelSequenceTriggerZone.generated.h"

UCLASS()
class PRACTICEGAME_API ALevelSequenceTriggerZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelSequenceTriggerZone();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent>	mTriggerZone;

	UPROPERTY(EditAnywhere)
	TObjectPtr<ULevelSequence>	mSequence;

	UPROPERTY(EditAnywhere)
	FMovieSceneSequencePlaybackSettings	mPlaySetting;

	TObjectPtr<ULevelSequencePlayer> mSequencePlayer;
	TObjectPtr<ALevelSequenceActor>	mSequenceActor;
	
	//역재생 해야하는 경우 체크
	UPROPERTY(EditAnywhere)
	bool		mIsReversePlay = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
public:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex);

	//레벨시퀀스 재생 종료 후 호출할 함수
	UFUNCTION()
	void SequenceOnFinishPlay();
};
