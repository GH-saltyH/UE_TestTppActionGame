// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "../Data/Item/ItemDataCacheManager.h"
#include "../UI/Character/Panel/PlayerInventoryFixedSlot.h"
#include "../UI/WidgetManager.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//InitializeComponent �̺�Ʈ ���ν����� ó���Ϸ��� �� ���� ���̾���Ѵ�
	bWantsInitializeComponent = true;

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// �κ��丮 ������ �����´�
	UPlayerInventoryFixedSlot* InventoryWidget = 
		CWidgetManager::GetInst()->FindWidget<UPlayerInventoryFixedSlot>(TEXT("PlayerInventoryFixedSlot"));

	//�κ��丮 ������ �� Ŭ������ ���� ������ �ʱ�ȭ�Ѵ� -> ���� �����ϰ� ����� ������ �־�� �����Ͱ� �ݿ��� ���̴�.
	if (InventoryWidget)
	{
		InventoryWidget->InitInventory(this);
	}
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();

	//�κ��丮�� ������ �̸� ��ĭ���� ��ü �޸𸮻���� �Ҵ��صд�
	mInvItemList.Init(nullptr, mLimitSlot);
}

bool UInventoryComponent::AddItem(const FName& ItemName)
{
	//���� ó�� -> �̹� ���� ��� �н� 
	if (mAddedItemCount == mLimitSlot)
	{
		UE_LOG(TestGame, Warning, TEXT("Add Item failure : Inventory is Full"));
		return false;
	}

	//������ ���̺� ĳ��
	UDataTable* MasterTable = UItemDataCacheManager::GetInst()->GetItemMasterTable();

	//���� ���� �������� �߰��Ѵ�
	UE_LOG(TestGame, Warning, TEXT("Generating Item Target = [%s]"), *ItemName.ToString());

	// ������ ���� ���� �ش� Ű������ �� ���ڵ带 �о�´�
	const FItemMasterRow* FindMasterRow = MasterTable->FindRow<FItemMasterRow>(ItemName, TEXT(""));

	//������ �α׸� ����ϰ� �������� �ݺ��Ѵ�
	if (!FindMasterRow)
	{
		UE_LOG(TestGame, Warning, TEXT("Add Item Failed! Cannot Find Item [%s] In Master Data Table."), *ItemName.ToString());
		return false;
	}

	// ������ ���̺��� DataTableRef ���� ������ ���� ������ ���̺� �ε��Ѵ�  
	// (���ε����� ���̺� = Weapon / Armor / Consumable ������� �Ļ�)
	// LoadSynchronous �� ���� �����̴�.  
	// �����Ͱ� ���� �޸𸮿� �ε�Ǿ����� �ʾҴ��� ��θ� ����ϰ� �ִٰ� �ʿ��� �� �ε��� �� �ְ� �ȴ�.
	UDataTable* DetailTable = FindMasterRow->DataTableRef.LoadSynchronous();

	if (!DetailTable)
	{
		UE_LOG(TestGame, Warning, TEXT("Item Add failed during Getting Detail Table"));
		return false;
	}

	//����� ������ ���̺�κ��� �� �� �о�´� 
	FTableRowBase* DetailData = DetailTable->FindRow<FTableRowBase>(FindMasterRow->RefRowName, TEXT(""));
	if (!DetailData)
	{
		UE_LOG(TestGame, Warning, TEXT("Adding Item : Failed to find Specific Item name [%s] in detail data table"), *ItemName.ToString());
		return false;
	}

	// ������ ������ ����
	UInvItemData* Item = NewObject<UInvItemData>();
	Item->InitializeItem(ItemName, *FindMasterRow, DetailData);
	
	//������� ���� ������ �������� �⺻ �ڷ���� �غ�Ǿ��� (���������̺�κ��� ���̸��� ã�Ƽ� �� ������ Item ��ü�� ��� �޾Ƶξ���)
	//���� �� �κ��丮 ������Ʈ�� ���� ī��Ʈ ���� ��ȣ�� �߰��ϱ⸸ �ϸ� �ȴ�.  -> �ʱ�ȭ���� �̹� ����Ʈ��ŭ�� �κ��丮 �迭�� �Ҵ��صξ��� ������ �̹���� ��� ��3��

	//�κ��丮�� ����Ž���Ͽ� ����ִ� ���̸� �������� �߰��ϰ� �����Ѵ�
	for (int32 i = 0; i < mLimitSlot; ++i)
	{
		if (!IsValid(mInvItemList[i]))
		{
			mInvItemList[i] = Item;

			//�ٿ��� ������ ���Ű� ���õ� ��� ��������Ʈ  ��ε��ɽ���
			if (mInvChangeDelegate.IsBound())
				mInvChangeDelegate.Broadcast(Item, i);
			++mAddedItemCount;
			break;
		}
	}
	UE_LOG(TestGame, Log, TEXT("Adding Item Succeed !  --- SlotNum[%d] item [%s]"), mAddedItemCount, *ItemName.ToString());

	return true;
}

void UInventoryComponent::UseItem(int32 Index)
{
}

void UInventoryComponent::SwapItem(int32 PrevIndex, int32 ToChangeIndex)
{
	mInvItemList.Swap(PrevIndex, ToChangeIndex);

	//������ ������ ������ ���濡 �����ϴ� ��������Ʈ �Լ� ��ε��ɽ�Ʈ 
	// -> ���� ��������Ʈ ���ε��� �Լ��� ���� ������ �������� ���ư��� ������  �ٲ� �ε��� ��� ������� ��
	if (mInvChangeDelegate.IsBound())
	{
		mInvChangeDelegate.Broadcast(mInvItemList[PrevIndex], PrevIndex);
		mInvChangeDelegate.Broadcast(mInvItemList[ToChangeIndex], ToChangeIndex);
	}
}

void UInventoryComponent::RemoveItem(int32 Index)
{

	//���� ó�� ������ ī��Ʈ�� 0�ΰ�� ����ó�� �н�
	if (mAddedItemCount == 0)
	{
		UE_LOG(TestGame, Warning, TEXT("there isn't any item to be delete in you current inventory"));
		return;
	}
	mInvItemList[Index] = nullptr;

	--mAddedItemCount;

	//������ ���� ��������Ʈ ���ε� ��ε�ĳ��Ʈ�� ���� ����
	if (mInvChangeDelegate.IsBound())
	{
		mInvChangeDelegate.Broadcast(nullptr, Index);
	}
}

bool UInventoryComponent::CheckItem(int32 Index)
{
	//return IsValid(mInvItemList[Index]);  �Ʒ��� ���� 
	return mInvItemList[Index] != nullptr;
}

