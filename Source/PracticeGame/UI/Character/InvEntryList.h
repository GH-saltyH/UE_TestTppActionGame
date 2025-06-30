// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../UIInfo.h"
#include "../../GameInfo.h"
#include "Blueprint/UserWidget.h"
#include "InvEntryList.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API UInvEntryList : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UInvEntryList(const FObjectInitializer& ObjectInitializer);
	
protected:
	TObjectPtr<UImage>		mItemIcon;
	TObjectPtr<UTextBlock>	mItemName;
	TObjectPtr<UTextBlock>	mItemPower;
	TObjectPtr<UTextBlock>	mItemType;
	TObjectPtr<UTextBlock>	mItemRarity;
	TObjectPtr<UTextBlock>	mDetailInfoByType;

protected:
	virtual void NativeOnInitialized();

public:
	//������ ������ �������� 
	UFUNCTION(BlueprintCallable)
	void SetItemData(UObject* Data);


	//�̺�Ʈ ��������Ʈ �Լ� ����
public:

	// ���� �ʿ��� ���� ������ �������� �ε��ϰ� ǥ���� �� �ְ� ó�����ش�
	//void UInvItemSlot::NativeOnListItemObjectSet(UObject* ListItemObject)
	//{
	//	UInvItemData* ItemData = Cast<UInvItemData>(ListItemObject);
	//	if (!ItemData) return;

	//	// ������ �����Ϳ� �����ؼ� UI ����
	//	ItemNameText->SetText(ItemData->GetMasterData().DisplayName);
	//	ItemIcon->SetBrushFromTexture(ItemData->GetMasterData().Icon);
	//}

};
