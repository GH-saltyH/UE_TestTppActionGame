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
	//������ ���� 
	//������ ���̺��� �� �̸��̴�. �������̾�߸� �Ѵ�
	FName	mItemID;
	//������ ���̺��� �������̴� -> ���ε����͵� �ʿ��ϴ�
	FItemMasterRow	mItemMasterData;
	//�����͸� �������� ����� ���� �������̴� (���� / �� / �Һ��ǰ �� �������� ���̺�� ������)
	TVariant<FWeaponData, FArmorData, FConsumableData> mDetailData;

public:
	//Ÿ�Կ� �����ο� ������ ������ ���ٿ� �Լ� -> ��� ������ ������ ���̺� ����
	template<typename T>
	const T* GetDetailDataAs() const
	{
		if (mDetailData.IsType<T>())
		{
			return &mDetailData.Get<T>();
		}
		return nullptr;
	}

	//�� ������ ��ü�� �����͵����� �޾ƿ��� 
	const FItemMasterRow& GetMasterData() const 
	{ 
		return mItemMasterData; 
	}

	//������ID �޾ƿ���
	FName GetItemID() const
	{ 
		return mItemID; 
	}

public:

	//������ ���� �� �ʿ� �޼��� -> �����ͺ��̽��κ��� ���޹��� �ο췹�ڵ�κ��� �� ���� ������ �����͸� ������
	//���� ���� �� �����ͷ� �������� �����Ѵ�,  ������ �����Ϳ� ������ �����͸� �޴´�
	//UFUNCTION()
	void InitializeItem(const FName& InItemID,
		const FItemMasterRow InMasterRow,
		const FTableRowBase* InDetailRow
	);
};
