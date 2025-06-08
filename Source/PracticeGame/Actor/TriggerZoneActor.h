// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "GameFramework/Actor.h"
#include "TriggerZoneActor.generated.h"

UCLASS()
class PRACTICEGAME_API ATriggerZoneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATriggerZoneActor();

private:
	//����ƽ �޽� ������Ʈ�� ��� ���� ����̴�
	//���� �����߿� �� �� �ִ�
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mStaticMesh;

	//�ڽ�������Ʈ�� ��� ���� �����
	//���� ���� �߿� �� �� �ִ�
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent>	mTriggerBox;

	//�� ����� Overlap �� Ʈ���ŵǸ� true �� �Ҵ��Ѵ�
	bool bIsTriggered = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	//�� ��������Ʈ �Լ��� 
	// mTriggerBox �� Overlap �̺�Ʈ �߻� ��
	// ȣ���ϰ� �ȴ�
	UFUNCTION()
	void TriggerBoxOnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
