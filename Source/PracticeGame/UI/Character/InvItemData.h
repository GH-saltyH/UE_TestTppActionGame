// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UIInfo.h"
#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "InvItemData.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UInvItemData : public UObject
{
	GENERATED_BODY()
	
	
public:
	UInvItemData();


private:
	//아이템 구조 
	//아이템 테이블의 행 이름이다. 독립적이어야만 한다
	FName	mItemID;
	//마스터 테이블의 데이터이다 -> 세부데이터도 필요하다
	FItemMasterRow	mItemMasterData;
	//마스터를 기준으로 연결된 세부 데이터이다 (무기 / 방어구 / 소비용품 등 세부적인 테이블로 나뉜다)
	TVariant<FWeaponData, FArmorData, FConsumableData> mDetailData;

public:
	//타입에 자유로운 디테일 데이터 접근용 함수 -> 모든 유형의 디테일 테이블에 대응
	template<typename T>
	const T* GetDetailDataAs() const
	{
		if (mDetailData.IsType<T>())
		{
			return &mDetailData.Get<T>();
		}
		return nullptr;
	}

	//각 아이템 객체의 마스터데이터 받아오기 
	const FItemMasterRow& GetMasterData() const 
	{ 
		return mItemMasterData; 
	}

	//아이템ID 받아오기
	FName GetItemID() const
	{ 
		return mItemID; 
	}

public:

	//아이템 생성 시 필요 메서드 -> 데이터베이스로부터 전달받은 로우레코드로부터 한 번에 아이템 데이터를 만들어낸다
	//전달 받은 행 데이터로 아이템을 설정한다,  마스터 데이터와 디테일 데이터를 받는다
	//UFUNCTION()
	void InitializeItem(const FName& InItemID,
		const FItemMasterRow InMasterRow,
		const FTableRowBase* InDetailRow
	);
};
