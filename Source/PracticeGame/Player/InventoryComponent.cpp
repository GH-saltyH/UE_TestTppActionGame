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

	//InitializeComponent 이벤트 프로시저를 처리하려면 이 값이 참이어야한다
	bWantsInitializeComponent = true;

}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// 인벤토리 위젯을 가져온다
	UPlayerInventoryFixedSlot* InventoryWidget = 
		CWidgetManager::GetInst()->FindWidget<UPlayerInventoryFixedSlot>(TEXT("PlayerInventoryFixedSlot"));

	//인벤토리 위젯을 이 클래스의 값을 가지고 초기화한다 -> 서로 연결하고 저장된 파일이 있어야 데이터가 반영될 것이다.
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

	//인벤토리의 슬롯을 미리 빈칸으로 전체 메모리사이즈를 할당해둔다
	mInvItemList.Init(nullptr, mLimitSlot);
}

bool UInventoryComponent::AddItem(const FName& ItemName)
{
	//예외 처리 -> 이미 꽉찬 경우 패스 
	if (mAddedItemCount == mLimitSlot)
	{
		UE_LOG(TestGame, Warning, TEXT("Add Item failure : Inventory is Full"));
		return false;
	}

	//마스터 테이블 캐싱
	UDataTable* MasterTable = UItemDataCacheManager::GetInst()->GetItemMasterTable();

	//전달 받은 아이템을 추가한다
	UE_LOG(TestGame, Warning, TEXT("Generating Item Target = [%s]"), *ItemName.ToString());

	// 마스터 정보 에서 해당 키워드의 행 레코드를 읽어온다
	const FItemMasterRow* FindMasterRow = MasterTable->FindRow<FItemMasterRow>(ItemName, TEXT(""));

	//없으면 로그를 출력하고 다음으로 반복한다
	if (!FindMasterRow)
	{
		UE_LOG(TestGame, Warning, TEXT("Add Item Failed! Cannot Find Item [%s] In Master Data Table."), *ItemName.ToString());
		return false;
	}

	// 마스터 테이블의 DataTableRef 값을 가지고 세부 데이터 테이블 로딩한다  
	// (세부데이터 테이블 = Weapon / Armor / Consumable 등등으로 파생)
	// LoadSynchronous 는 약한 참조이다.  
	// 데이터가 아직 메모리에 로드되어있지 않았더라도 경로만 기억하고 있다가 필요할 때 로드할 수 있게 된다.
	UDataTable* DetailTable = FindMasterRow->DataTableRef.LoadSynchronous();

	if (!DetailTable)
	{
		UE_LOG(TestGame, Warning, TEXT("Item Add failed during Getting Detail Table"));
		return false;
	}

	//연결된 디테일 테이블로부터 한 줄 읽어온다 
	FTableRowBase* DetailData = DetailTable->FindRow<FTableRowBase>(FindMasterRow->RefRowName, TEXT(""));
	if (!DetailData)
	{
		UE_LOG(TestGame, Warning, TEXT("Adding Item : Failed to find Specific Item name [%s] in detail data table"), *ItemName.ToString());
		return false;
	}

	// 아이템 데이터 생성
	UInvItemData* Item = NewObject<UInvItemData>();
	Item->InitializeItem(ItemName, *FindMasterRow, DetailData);
	
	//여기까지 오면 생성할 아이템의 기본 자료들이 준비되었음 (데이터테이블로부터 행이름을 찾아서 그 원형을 Item 객체에 모두 받아두었음)
	//이제 이 인벤토리 컴포넌트의 현재 카운트 다음 번호에 추가하기만 하면 된다.  -> 초기화에서 이미 리미트만큼의 인벤토리 배열을 할당해두었기 때문에 이방식을 써야 한3다

	//인벤토리를 순차탐색하여 비어있는 곳이면 아이템을 추가하고 종료한다
	for (int32 i = 0; i < mLimitSlot; ++i)
	{
		if (!IsValid(mInvItemList[i]))
		{
			mInvItemList[i] = Item;

			//바운드된 아이템 갱신과 관련된 모든 델리게이트  브로드케스팅
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

	//변경이 있으니 아이템 변경에 의존하는 델리게이트 함수 브로드케스트 
	// -> 현재 델리게이트 바인딩된 함수는 개별 슬롯의 갱신으로 돌아가기 때문에  바뀐 인덱스 모두 돌려줘야 함
	if (mInvChangeDelegate.IsBound())
	{
		mInvChangeDelegate.Broadcast(mInvItemList[PrevIndex], PrevIndex);
		mInvChangeDelegate.Broadcast(mInvItemList[ToChangeIndex], ToChangeIndex);
	}
}

void UInventoryComponent::RemoveItem(int32 Index)
{

	//예외 처리 아이템 카운트가 0인경우 삭제처리 패스
	if (mAddedItemCount == 0)
	{
		UE_LOG(TestGame, Warning, TEXT("there isn't any item to be delete in you current inventory"));
		return;
	}
	mInvItemList[Index] = nullptr;

	--mAddedItemCount;

	//삭제에 대한 델리게이트 바인딩 브로드캐스트로 정보 갱신
	if (mInvChangeDelegate.IsBound())
	{
		mInvChangeDelegate.Broadcast(nullptr, Index);
	}
}

bool UInventoryComponent::CheckItem(int32 Index)
{
	//return IsValid(mInvItemList[Index]);  아래와 같음 
	return mInvItemList[Index] != nullptr;
}

