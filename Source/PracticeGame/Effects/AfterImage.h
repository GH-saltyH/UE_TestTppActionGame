// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/Actor.h"
#include "AfterImage.generated.h"

UCLASS()
class PRACTICEGAME_API AAfterImage : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAfterImage();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent>							mRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPoseableMeshComponent>					mMesh;

	TObjectPtr<UMaterialInterface>						mOriginMaterial;
	TArray<TObjectPtr<UMaterialInstanceDynamic>>		mMaterials;

	float												mFadeTime = 1.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void CopyImage(USkeletalMeshComponent* Dest);

	void SetOriginMaterial(const FString& Path);
	void SetOriginMaterial(UMaterialInterface* Material);
	
private:
	//���� �ذ��� ���� �������� ���ķ� ��Ƽ������ �����ϰԲ� �Ѵ�.
	void UpdateCopyMesh();
};
