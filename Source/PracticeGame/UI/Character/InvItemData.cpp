// Fill out your copyright notice in the Description page of Project Settings.


#include "InvItemData.h"
//UInvItemData* UInvItemData::mDefaultIcon = nullptr;

UInvItemData::UInvItemData()
{

}

void UInvItemData::InitializeItem(const FName& InItemID, const FItemMasterRow InMasterRow, const FTableRowBase* InDetailRow)
{
	//���� ID ����
	mItemID = InItemID;
	//���������� �ѹ��� ����
	mItemMasterData = InMasterRow;

	//Ÿ�Ժ� ���� �� ���� �����ϱ� -> Ÿ�Ժ��� �ٸ� ����ü
	switch (mItemMasterData.ItemType)
	{
	case EItemType::WEAPON:
		if (const FWeaponData* Weapon = static_cast<const FWeaponData*>(InDetailRow))
		{
			/*mDetailData = *Weapon;*/
			mDetailData.Set<FWeaponData>(*Weapon);
		}
		break;

	case EItemType::ARMOR:
		if (const FArmorData* Armor = static_cast<const FArmorData*>(InDetailRow))
		{
			mDetailData.Set<FArmorData>(*Armor);
		}
		break;

	case EItemType::CONSUMABLE:
		if (const FConsumableData* Consumable = static_cast<const FConsumableData*>(InDetailRow))
		{
			mDetailData.Set<FConsumableData>(*Consumable);
		}
		break;

	default:
		UE_LOG(LogTemp, Warning, TEXT("InitializeItem: Unknown ItemType encountered for [%s]"), *InItemID.ToString());
		break;
	}

	//�ش� ������ �ܺο��� ������ �ڵ�
	/*if (mDetailData.IsType<FWeaponData>())
	{
		const FWeaponData& Data = mDetailData.Get<FWeaponData>();
		float Damage = Data.AttackPower;
	}*/
}
