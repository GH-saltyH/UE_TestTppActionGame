// Fill out your copyright notice in the Description page of Project Settings.


#include "MediaActor.h"


// Sets default values
AMediaActor::AMediaActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	//미디어사운드컴포넌트를 추가한다
	mMediaSound = CreateDefaultSubobject<UMediaSoundComponent>(TEXT("MediaSound"));

	SetRootComponent(mMesh);
	mMediaSound->SetupAttachment(mMesh);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>	StaticMeshAsset(TEXT("/Script/Engine.StaticMesh'/Engine/EditorMeshes/EditorPlane.EditorPlane'"));
	if (StaticMeshAsset.Succeeded())
		mMesh->SetStaticMesh(StaticMeshAsset.Object);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MtInterface(TEXT("/Script/Engine.Material'/Game/Movies/MTMediaPlayer.MTMediaPlayer'"));
	if (MtInterface.Succeeded())
	{
		mMediaMaterial = MtInterface.Object;
		mMesh->SetMaterial(0, mMediaMaterial);
	}



}

void AMediaActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	mMediaSound->SetMediaPlayer(mMediaPlayer);
	mMesh->SetMaterial(0, mMediaMaterial);
}

// Called when the game starts or when spawned
void AMediaActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMediaActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

