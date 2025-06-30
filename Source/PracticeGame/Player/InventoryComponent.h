// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "Components/ActorComponent.h"
#include "../UI/Character/InvItemData.h"
#include "InventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FInventoryChange, UInvItemData*, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FChangeGold, int32);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICEGAME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

private:
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<class UInvItemData>>	mInvItemList;
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32									mLimitSlot=50;
	// TArray 인데도 카운트가 필요한 이유는 초기화에서 리스트를 미리 최대크기로 메모리에 생성할 것이기 때문이다.
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32									mAddedItemCount = 0;
	
public:
	FInventoryChange	mInvChangeDelegate;
	FChangeGold			mChangeGoldDelegate;
	
public:
	int32 GetInvSlotLimit() const
	{
		return mLimitSlot;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void InitializeComponent();
		
public:
	bool AddItem(const FName& ItemName);
	void UseItem(int32 Index);
	void SwapItem(int32 PrevIndex, int32 ToChangeIndex);
	void RemoveItem(int32 Index);

	void SetGold(int32 Gold)
	{
		if (mChangeGoldDelegate.IsBound())
			mChangeGoldDelegate.Broadcast(Gold);
	}

	bool CheckItem(int32 Index);

public:
	UInvItemData* GetItem(int32 SlotIndex)
	{
		return mInvItemList[SlotIndex];
	}
};
