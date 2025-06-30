// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Pawn.h"
#include "Animation/SelectPawnAnimInstanceTemplate.h"
#include "../Interface/CharacterPublicInterface.h"
#include "SelectPawn.generated.h"

UCLASS()
class PRACTICEGAME_API ASelectPawn : public APawn, public ICharacterPublicInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASelectPawn();

protected:
	//�ٵ�
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent>	mBody;

	//���̷�Ż�޽�
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent>		mMesh;

	//�������� ������ �־�� �� -> �����Ϳ��� ��ġ �� �����صα�
	UPROPERTY(EditAnywhere)
	FName	mDataKey;
	UPROPERTY(EditAnywhere)
	EPlayerJob					mJob;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> mImageMtrl;

	TObjectPtr<USelectPawnAnimInstanceTemplate>	mAnimInst;

public:
	UMaterialInterface* GetImageMaterial()	const
	{
		return mImageMtrl;
	}
	virtual void OnConstruction(const FTransform& Transform);

public:
	EPlayerJob	GetJob() const
	{
		return mJob;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void EnableMeshStencil(bool Enable);
	void ChangeAnim(ESelectAnim Anim);
	
};
