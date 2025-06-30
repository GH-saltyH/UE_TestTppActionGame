// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "ItemDataCacheManager.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UItemDataCacheManager : public UObject
{
	GENERATED_BODY()

private:
	UItemDataCacheManager();

	//UObject 의 싱글턴 패턴 
private:
	static UItemDataCacheManager* mInst;

public:
	static UItemDataCacheManager* GetInst()
	{
		if (!mInst)
		{
			mInst = NewObject<UItemDataCacheManager>();
			mInst->AddToRoot(); // GC 방지
		}
		return mInst;
	}

	static void DestroyInst()
	{
		if (mInst)
		{
			mInst->RemoveFromRoot(); // GC에 다시 맡김
			mInst = nullptr;
		}
	}

private:
	//런타임 로드 시 UProPerty 다붙여줘야  GC 에 의해 해제를 막을 수 있음
	//UPROPERTY()
	UDataTable* mItemMasterTable;
	//UPROPERTY()
	UDataTable* mWeaponDataTable;
	//UPROPERTY()
	UDataTable* mArmorDataTable;
	//UPROPERTY()
	UDataTable* mConsumableDataTable;

public:
	void Initialize(UDataTable* ItemMasterTable);

public:
	UDataTable* GetItemMasterTable() const
	{
		return mItemMasterTable;
	}

	UDataTable* GetWeaponDataTable() const
	{
		return mWeaponDataTable;
	}

	UDataTable* GetArmorDataTable() const
	{
		return mArmorDataTable;
	}

	TArray<FName>	GetAllItemRowNames() const
	{
		TArray<FName> RowNames;

		if (mItemMasterTable)
		{
			RowNames = mItemMasterTable->GetRowNames();
		}

		return RowNames;
	}

	FName GetRandomItemRowName() const
	{
		if (!mItemMasterTable)
		{
			return NAME_None;
		}

		const TArray<FName>& RowNames = mItemMasterTable->GetRowNames();

		if (RowNames.Num() == 0)
		{
			return NAME_None;
		}

		int32 RandIndex = FMath::RandRange(0, RowNames.Num() - 1);
		return RowNames[RandIndex];
	}

};
