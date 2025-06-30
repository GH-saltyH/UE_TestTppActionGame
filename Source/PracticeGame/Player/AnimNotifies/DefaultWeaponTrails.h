// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "../PlayerCharacter.h"             //�ε��� Ʈ���� ��ƼŬ�� �������� ����
#include "Animation/AnimNotifies/AnimNotifyState_Trail.h"
#include "DefaultWeaponTrails.generated.h"

UCLASS()
class PRACTICEGAME_API UDefaultWeaponTrails : public UAnimNotifyState_Trail
{
	GENERATED_BODY()
	
protected:
    bool bDebugMode = false;

public:
    // ��ƼŬ ���� ����� �θ��� PSTemplate �� ���� �ȴ�
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trail)
    TObjectPtr<UParticleSystem> PSTemplate;*/

    // Ʈ������ ������ ���� �̸� -> ���ʿ�
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    FName SocketName;*/
    
    // ���� �̸��� �θ��� �͵��� ���� �ȴ�
    /** Name of the first socket defining this trail. */
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trail)
    FName FirstSocketName;*/

    /** Name of the second socket defining this trail. */
    /*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Trail)
    FName SecondSocketName;*/
    
    //TMap ���� ������Ʈ�� ���ӽð�(float) �� Ű������ �Ǿ� �ִ�
    TMap<USceneComponent*, float>   ActiveTrailEffect;  

    //Ʈ���� ��Ƽ���� ���� �� ȣ��
    virtual void NotifyBegin(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        float TotalDuration,
        const FAnimNotifyEventReference& EventReference)         override;

    //Ʈ���� ��Ƽ���� ���� �� ȣ��
    virtual void NotifyEnd(
        USkeletalMeshComponent* MeshComp,
        UAnimSequenceBase* Animation,
        const FAnimNotifyEventReference& EventReference)        override;
};
