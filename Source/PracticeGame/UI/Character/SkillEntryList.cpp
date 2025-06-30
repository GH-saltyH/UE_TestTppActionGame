// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillEntryList.h"
#include "SkillListData.h"
#include "../../Player/PlayerCharacter.h"



USkillEntryList::USkillEntryList(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
}

void USkillEntryList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mSkillIcon = Cast<UImage>(GetWidgetFromName(TEXT("SkillIcon")));
	mSkillName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextSkillName")));
	mSkillInfo = Cast<UTextBlock>(GetWidgetFromName(TEXT("TextSkillInfo")));
}

void USkillEntryList::NativeConstruct()
{
	Super::NativeConstruct();

	////데이터테이블로 부터 스킬들을 읽어온다
	//UDataTable* DTSkillInfo = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Data/Player/Skills/DataTable/DT_PlayerSkillInfo.DT_PlayerSkillInfo'"));
	//
	//if (DTSkillInfo)
	//{
	//	//먼저 모두 받는다
	//	TArray<FPlayerSkillData*>	AllRows;
	//	DTSkillInfo->GetAllRows(TEXT("SkillRowLookup"), AllRows);

	//	APlayerController* PC = GetOwningPlayer();
	//	if (PC)
	//	{
	//		APlayerCharacter* Player = Cast<APlayerCharacter>(PC->GetPawn());
	//		if (Player)
	//		{
	//			UE_LOG(TestGame, Warning, TEXT("SkillEntryList : Get Player succeed!"));
	//			EPlayerJob	CurrentJob = Player->GetJob();

	//			//모든 행에서 클래스 타입이 맞는 것만 가져온다
	//			for (FPlayerSkillData* Row : AllRows)
	//			{
	//				mFilteredSkillData.Add(*Row);
	//			}
	//		}
	//	}
	//}
}

void USkillEntryList::SetSkillData(UObject* Data)
{
	USkillListData* SkillData = Cast<USkillListData>(Data);

	if (SkillData)
	{
		mSkillIcon->SetBrushFromTexture(SkillData->GetSkillData().Icon);
		mSkillName->SetText(FText::FromString(SkillData->GetSkillData().Name));
	}
}