// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../PlayerCharacter.h"             //로드한 트레일 파티클을 가져오기 위함
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "DefaultWeaponTrails.generated.h"

UCLASS()
class PRACTICEGAME_API UDefaultWeaponTrails : public UAnimNotifyState_Trail
{
	GENERATED_BODY()
	
protected:
    bool bDebugMode = false;

public:
    // 파티클 에셋 멤버는 부모의 PSTemplate 를 쓰면 된다
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trail)
    TObjectPtr<UParticleSystem> PSTemplate;*/

    // 트레일을 적용할 소켓 이름 -> 불필요
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    FName SocketName;*/
    
    // 소켓 이름은 부모의 것들을 쓰면 된다
    /** Name of the first socket defining this trail. */
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trail)
    FName FirstSocketName;*/

    /** Name of the second socket defining this trail. */
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trail)
    FName SecondSocketName;*/
    
    //TMap 으로 컴포넌트와 지속시간(float) 의 키쌍으로 되어 있다
    TMap<USceneComponent*, float>   ActiveTrailEffect;  

    //트레일 노티파이 시작 시 호출
    virtual void NotifyBegin(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        float TotalDuration,
        const FAnimNotifyEventReference& EventReference)         override;

    //트레일 노티파이 종료 시 호출
    virtual void NotifyEnd(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        const FAnimNotifyEventReference& EventReference)        override;
};
