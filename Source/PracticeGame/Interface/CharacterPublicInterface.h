// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UObject/Interface.h"
#include "CharacterPublicInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterPublicInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PRACTICEGAME_API ICharacterPublicInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
    //이 인터페이스를 다중 상속하는 클래스에서  해당 정보를 쓸 수 있다 -> 캐릭터의 상태값을 가지고 있는다
protected:
    EPlayerJob      mJob;
    FString			mName;                    
    float			mAttack;                    
    float			mDefense;                    
    float			mHPMax;
    float			mHP;
    float			mMP;
    float			mMPMax;
    float			mAttackDistance;                     
    float			mMoveSpeed;                    
    int32			mLevel;           
    int32           mGold;
    int32           mExp;

    //각 멤버를 외부에서 통제할 수 있도록 함수를 정의한다
public:
    FString GetJobString() const
    {
        if (const UEnum* EnumPtr = StaticEnum<EPlayerJob>())
        {
            UE_LOG(TestGame, Warning, TEXT("Get Name String By Value : EPlayerJob "));
            return EnumPtr->GetNameStringByValue(static_cast<int64>(mJob));
        }

        return TEXT("Invalid Job");
    }

    EPlayerJob  GetJob()    const
    {
        return mJob;
    }

    FString GetName() const
    {
        return mName;
    }

    float GetAttack()   const
    {
        return mAttack;
    }

    float GetDefense()   const
    {
        return mDefense;
    }

    float GetHP()   const
    {
        return mHP;
    }

    float GetMaxHP()   const
    {
        return mHPMax;
    }

    float GetMP()   const
    {
        return mMP;
    }

    float GetMaxMP()   const
    {
        return mMPMax;
    }

    float GetAttackDistance()   const
    {
        return mAttackDistance;
    }

    float GetMoveSpeed()   const
    {
        return mMoveSpeed;
    }

    int32 GetLevel()    const
    {
        return mLevel;
    }

    int32 GetGold()    const
    {
        return mGold;
    }

    int32 GetExp()    const
    {
        return mExp;
    }

public:
    void SetName(const FString& NewName)
    {
        mName = NewName;
    }

    void SetAttack(float NewAttack)
    {
        mAttack = NewAttack;
    }
    void SetDefense(float NewDefense)
    {
        mDefense = NewDefense;
    }

    bool Damage(float Dmg)
    {
        float PostDmg = mHP - Dmg;
        mHP = PostDmg <  0.f ? 0.f : PostDmg;

        return mHP <= 0.f;
    }

    void AddMP(float AddMP)
    {
       mMP = FMath::Max(0, mMP + AddMP);
    }

    void SetAttackDistance(float NewAttackDistance)
    {
        mAttackDistance = NewAttackDistance;
    }

    void SetMoveSpeed(float NewSpeed)
    {
        mMoveSpeed = NewSpeed;
    }

    void SetLevel(int32 NewLevel)
    {
        mLevel = NewLevel;
    }

    void AddExp(int32 AddNumber)
    {
        mExp += AddNumber;
    }

    void AddGold(int32 AddGold)
    {
        mGold += AddGold;
    }

    void AddLevel(int32 AddLevel)
    {
        mLevel += AddLevel;
    }
};
