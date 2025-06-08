// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EngineMinimal.h"

//TestGame ���� �α׼���
DECLARE_LOG_CATEGORY_EXTERN(TestGame, Log, All);

//����� �ִϸ��̼��� �����ϱ� ���� ����ϴ� �������̴�
UENUM(BlueprintType)
enum class EPlayerAnim : uint8	
{
	Ground,
	Jump,
	Falling,
	MoveXY,
	Landed
};

//�ɸ��� ���������� �����ϴ� ����ü
struct FCharacterState
{
    uint8 StateFlags;  // �ִ� 8���� ���� ���� ���� (8bit)

    enum ECharacterFlags
    {
        STUNNED              = 1 << 0,           // 00000001
        IMMOBILIZED         = 1 << 1,           // 00000010
        INVULNERABLE      = 1 << 2,           // 00000100
        ACTING                 = 1 << 3,            // 00001000
        LANDING               = 1 << 4,            // 00010000
        COMBO                 = 1 << 5              // 00100000
    };

    //�ش� ���¸� ���� �ִ���=true ��ȯ�Ѵ�
    bool HasState(uint8 Flag) const
    {
        return (StateFlags & Flag) != 0;
    }

    //���¸� �����Ѵ� -> 1
    void SetState(uint8 Flag)
    {
        StateFlags |= Flag;
    }

    //���¸� �����Ѵ� -> 0
    void ClearState(uint8 Flag)
    {
        StateFlags &= ~Flag;
    }
};
