// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EngineMinimal.h"

//TestGame 관련 로그설정
DECLARE_LOG_CATEGORY_EXTERN(TestGame, Log, All);

//재생할 애니메이션을 결정하기 위해 사용하는 열거형이다
UENUM(BlueprintType)
enum class EPlayerAnim : uint8	
{
	Ground,
	Jump,
	Falling,
	MoveXY,
	Landed
};

//케릭터 상태정보를 저장하는 구조체
struct FCharacterState
{
    uint8 StateFlags;  // 최대 8개의 상태 저장 가능 (8bit)

    enum ECharacterFlags
    {
        STUNNED              = 1 << 0,           // 00000001
        IMMOBILIZED         = 1 << 1,           // 00000010
        INVULNERABLE      = 1 << 2,           // 00000100
        ACTING                 = 1 << 3,            // 00001000
        LANDING               = 1 << 4,            // 00010000
        COMBO                 = 1 << 5              // 00100000
    };

    //해당 상태를 갖고 있는지=true 반환한다
    bool HasState(uint8 Flag) const
    {
        return (StateFlags & Flag) != 0;
    }

    //상태를 설정한다 -> 1
    void SetState(uint8 Flag)
    {
        StateFlags |= Flag;
    }

    //상태를 해제한다 -> 0
    void ClearState(uint8 Flag)
    {
        StateFlags &= ~Flag;
    }
};
