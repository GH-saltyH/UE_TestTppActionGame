// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "GameFramework/Actor.h"
#include "MediaSoundComponent.h"
#include "MediaActor.generated.h"

UCLASS()
class PRACTICEGAME_API AMediaActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMediaActor();

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent>	mMesh;

	//미디어플레이어 
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMediaPlayer>			mMediaPlayer;

	//파일 미디어 소스
	UPROPERTY(EditAnywhere)
	TObjectPtr<UFileMediaSource>		mMediaSource;

	///사운드는 따로 재생시켜야 하므로 사운드 컴포넌트도 추가해준다 -> 나중에 미디어소스로부터 사운드를 받아서 동기화할 수 있게 한다
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMediaSoundComponent>	mMediaSound;

	//미디어플레이어 -> 프레임 텍스처로 출력 -> 출력된 텍스처를  머티리얼에서 다시 받아서 출력하게된다. 그 머티리얼의 주소이다
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface>		mMediaMaterial;

	//에디터에서 변경하면 재생을 기다릴 시간을 지정할 수 있다
	UPROPERTY(EditAnywhere)
	float	mDelayTime = 0.f;

	float mStartTime = 0.f;

	//재생을 시작하면 true 로 바꾸어 관련된 처리를 하도록 돕는다
	bool mStart = false;


public:
	void OnConstruction(const FTransform& Transform);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
