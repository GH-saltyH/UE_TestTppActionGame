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
	//스태틱 메쉬 컴포넌트를 들고 있을 멤버이다
	//게임 실행중에 볼 수 있다
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> mStaticMesh;

	//박스컴포넌트를 들고 있을 멤버다
	//게임 실행 중에 볼 수 있다
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBoxComponent>	mTriggerBox;

	//이 멤버는 Overlap 이 트리거되면 true 를 할당한다
	bool bIsTriggered = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	//이 델리게이트 함수는 
	// mTriggerBox 의 Overlap 이벤트 발생 시
	// 호출하게 된다
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
