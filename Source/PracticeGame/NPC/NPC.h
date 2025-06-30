// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "../Interface/CharacterRenderingInterface.h"
#include "GameFramework/Pawn.h"
#include "Components/WidgetComponent.h"
#include "../UI/MainGame/NPC/HPPlateWidget.h"
#include "NPC.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FNPCHPChange, float, float);
DECLARE_MULTICAST_DELEGATE_TwoParams(FNPCMPChange, float, float);

UCLASS()
class PRACTICEGAME_API ANPC : public APawn,
	public IGenericTeamAgentInterface,
	public ICharacterRenderingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ANPC();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent>	mBody;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent>	mMesh;

	//각 하위 클래스들이 데이터에셋에서 자기를 정의한 행데이터를 읽어올 때 필요한 FName 타입의 키문자이다
	UPROPERTY(EditAnywhere)
	FName	mDataKey;

	FGenericTeamId	 mTeamID;

	//위젯 컴포넌트  생명력 바를 붙여줄 컴포넌트
	UPROPERTY(EditAnywhere)
	UWidgetComponent* mWidgetComponent;
	TSubclassOf<UUserWidget> mHPPlateWidgetClass;		//HPPlate 위젯 클래스
	//UPROPERTY(VisibleAnywhere)
	//TObjectPtr<UHPPlateWidget> mHPPlateWidget;	//생명력 바 위젯
	
	FNPCHPChange	mNPCHPChangeDelegate;
	FNPCMPChange	mNPCMPChangeDelegate;

public:
	const virtual USkeletalMeshComponent* GetMeshComponent() const
	{
		return mMesh;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID);
	virtual FGenericTeamId GetGenericTeamId() const;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;

public:
	virtual void EnableOutLine(bool Enable);
	virtual void EnableOcclusion(bool Enable);
};
