// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemDataCacheManager.h"

UItemDataCacheManager* UItemDataCacheManager::mInst = nullptr;

UItemDataCacheManager::UItemDataCacheManager()
{
	//초기화 단계에서 데이터테이블을 읽어놓는다 -> 언제든 이 클래스르 통하여 꺼내올 수 있다.    
	//데이터 테이블 읽어놓기
	
	/*static const FString MasterPath = TEXT("/Game/Data/Items/DataTables/DT_ItemMaster.DT_ItemMaster");
	static const FString WeaponPath = TEXT("/Game/Data/Items/DataTables/DT_WeaponData.DT_WeaponData");
	static const FString ArmorPath = TEXT("/Game/Data/Items/DataTables/DT_ArmourData.DT_ArmourData");*/
	//static const FString ConsumablePath = TEXT("");
	
	//런타임 로드 방식
	/*mItemMasterTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *MasterPath));
	mWeaponDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *WeaponPath));
	mArmorDataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *ArmorPath));*/

	//생성자 로드방식
	static ConstructorHelpers::FObjectFinder<UDataTable> MasterDT(TEXT("/Script/Engine.DataTable'/Game/Data/Items/DataTables/DT_ItemMaster.DT_ItemMaster'"));
	if (MasterDT.Succeeded())
		mItemMasterTable = MasterDT.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> ArmorDT(TEXT("/Script/Engine.DataTable'/Game/Data/Items/DataTables/DT_ArmourData.DT_ArmourData'"));
	if (ArmorDT.Succeeded())
		mArmorDataTable = ArmorDT.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponDT(TEXT("/Script/Engine.DataTable'/Game/Data/Items/DataTables/DT_WeaponData.DT_WeaponData'"));
	if (WeaponDT.Succeeded())
		mWeaponDataTable = WeaponDT.Object;

	mConsumableDataTable = nullptr;

	if (!mItemMasterTable || !mWeaponDataTable || mArmorDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemDataTableCache: Failed to load one or more DataTables."));
	}
}


void UItemDataCacheManager::Initialize(UDataTable* ItemMasterTable)
{
   
}


