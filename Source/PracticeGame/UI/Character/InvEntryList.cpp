// Fill out your copyright notice in the Description page of Project Settings.


#include "InvEntryList.h"
#include "InvItemData.h"

UInvEntryList::UInvEntryList(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}


void UInvEntryList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mItemIcon = Cast<UImage>(GetWidgetFromName(TEXT("ItemIcon")));
	mItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	mItemPower = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemPower")));;
	mItemType = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemType")));;
	mItemRarity = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemRarity")));;
	mDetailInfoByType = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemMiscInfo")));;
}

void UInvEntryList::SetItemData(UObject* Data)
{
	UInvItemData* ItemData = Cast<UInvItemData>(Data);

	if (ItemData)
	{
		const FItemMasterRow& MasterData = ItemData->GetMasterData();
		if (!MasterData.DisplayName.IsEmpty())
		{
			//������ ������ ����
			mItemIcon->SetBrushFromTexture(MasterData.Icon);
			mItemName->SetText(MasterData.DisplayName);
			EItemRarity Rarity = MasterData.Rarity;
			switch (Rarity)
			{
			case EItemRarity::CRACKED:
				mItemRarity->SetText(FText::FromString(TEXT("Cracked")));
				break;
			case EItemRarity::NORMAL:
				mItemRarity->SetText(FText::FromString(TEXT("Normal")));
				break;
			case EItemRarity::MAGIC:
				mItemRarity->SetText(FText::FromString(TEXT("Magic")));
				break;
			case EItemRarity::RARE:
				mItemRarity->SetText(FText::FromString(TEXT("Rare")));
				break;
			case EItemRarity::UNIQUE:
				mItemRarity->SetText(FText::FromString(TEXT("Unique")));
				break;
			case EItemRarity::ARTIFACT:
				mItemRarity->SetText(FText::FromString(TEXT("Artifact")));
				break;
			}

			//������ ������ ���� 
			EItemType		ItemType = MasterData.ItemType;
			switch (ItemType)
			{
			case EItemType::WEAPON:
			{
				const FWeaponData* WeaponData = ItemData->GetDetailDataAs<FWeaponData>();
				if (WeaponData)
				{
					//���ݷ�
					FString OutputAttack = FString::Printf(TEXT("Damage : %0.1f"), WeaponData->AttackPower);
					mItemPower->SetText(FText::FromString(OutputAttack));

					//��������
					EItemWeaponType WeaponType = WeaponData->WeaponType;
					switch (WeaponType)
					{
					case EItemWeaponType::SWORDS:
						mItemType->SetText(FText::FromString(TEXT("SWORD")));
						break;
					case EItemWeaponType::AXES:
						mItemType->SetText(FText::FromString(TEXT("AXE")));
						break;
					case EItemWeaponType::MACES:
						mItemType->SetText(FText::FromString(TEXT("MACE")));
						break;
					case EItemWeaponType::BOWS:
						mItemType->SetText(FText::FromString(TEXT("BOW")));
						break;
					}

					//4��° ���� : ����� ���ݼӵ�
					FString OutputAS = FString::Printf(TEXT("Attack Speed : %0.2f/s"), WeaponData->AttackSpeed);
					mDetailInfoByType->SetText(FText::FromString(OutputAS));
				}
				break;
			}
			case EItemType::ARMOR:
			{
				const FArmorData* ArmorData = ItemData->GetDetailDataAs<FArmorData>();
				if (ArmorData)
				{
					//��
					FString Defense = FString::Printf(TEXT("Defense : %0.0f"), ArmorData->Defense);
					mItemPower->SetText(FText::FromString(Defense));
					//��Ÿ�� : ������ ���� ���������
					mItemType->SetText(FText::FromString(TEXT("Armor")));
					//������
					FString Durability = FString::Printf(TEXT("Durability : %0.0f"), ArmorData->Durability);
					mDetailInfoByType->SetText(FText::FromString(Durability));
				}
				break;
			}
			case EItemType::CONSUMABLE:
				break;
			case EItemType::TRADEGOODS:
				break;
			}
		}
	}
}