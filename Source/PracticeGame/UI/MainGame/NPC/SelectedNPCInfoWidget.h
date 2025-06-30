// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../UserWidgetBase.h"
#include "../../../Interface/CharacterPublicInterface.h"
#include "SelectedNPCInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICEGAME_API USelectedNPCInfoWidget : public UUserWidgetBase
{
	GENERATED_BODY()
	
public:
	USelectedNPCInfoWidget(const FObjectInitializer& ObjectInitializer);

protected:
	//TObjectPtr<UImage> mPlayerIcon;
	TObjectPtr<UTextBlock> mNPCName;
	TObjectPtr<UProgressBar> mHPBar;
	TObjectPtr<UTextBlock> mHPText;
	//TObjectPtr<UProgressBar> mMPBar;
	//TObjectPtr<UTextBlock> mMPText;
	//TObjectPtr<UProgressBar> mXPBar;
	//TObjectPtr<UTextBlock> mXPText;

protected:
	virtual void NativeOnInitialized() override;

public:
	void SetNPCName(const FString& NewName);
	void SetHPBar(float HP, float MaxHP);
	//void SetMPBar(float MP, float MaxMP);
	//void SetXPBar(int32 XP, int32 NextXP);

	void UpdateInfo(ICharacterPublicInterface* NPCInfo)
	{
		if (IsVisible())
		{
			if (mNPCName && mHPBar && mHPText)
			{
				mNPCName->SetText(FText::FromString(NPCInfo->GetName()));
				mHPBar->SetPercent(NPCInfo->GetHP() / NPCInfo->GetMaxHP());
				FString HPText = FString::Printf(TEXT("%0.0f / %0.0f"), NPCInfo->GetHP(), NPCInfo->GetMaxHP());
				mHPText->SetText(FText::FromString(HPText));
			}
			else
			{
				UE_LOG(TestGame, Warning, TEXT("NPC Info : UpdateInfo -> nullptr One or More Component"));
				return;
			}
		}
	}
};
