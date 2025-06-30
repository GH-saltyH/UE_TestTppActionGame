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
	//아이템 데이터 가져오기 
	UFUNCTION(BlueprintCallable)
	void SetItemData(UObject* Data);


	//이벤트 델리게이트 함수 선언
public:

	// 실제 필요할 때만 아이템 디테일을 로드하고 표기할 수 있게 처리해준다
	//void UInvItemSlot::NativeOnListItemObjectSet(UObject* ListItemObject)
	//{
	//	UInvItemData* ItemData = Cast<UInvItemData>(ListItemObject);
	//	if (!ItemData) return;

	//	// 마스터 데이터에 접근해서 UI 갱신
	//	ItemNameText->SetText(ItemData->GetMasterData().DisplayName);
	//	ItemIcon->SetBrushFromTexture(ItemData->GetMasterData().Icon);
	//}

};
