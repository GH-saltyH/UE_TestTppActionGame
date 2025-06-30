// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectInfoWidget.h"
#include "../Interface/CharacterPublicInterface.h"


USelectInfoWidget::USelectInfoWidget(const FObjectInitializer& ObjectInitializer) :
	UUserWidgetBase(ObjectInitializer)
{
	mWidgetName = TEXT("SelectInfoWidget");
}

void USelectInfoWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	mCharacterImage = Cast<UImage>(GetWidgetFromName(TEXT("CharacterImage")));
	mJob= Cast<UTextBlock>(GetWidgetFromName(TEXT("JobName")));
	mCharacterName = Cast<UTextBlock>(GetWidgetFromName(TEXT("CharacterName")));
	mAttack = Cast<UTextBlock>(GetWidgetFromName(TEXT("AttackPower")));
	mDefense = Cast<UTextBlock>(GetWidgetFromName(TEXT("Defense")));
	mHP= Cast<UTextBlock>(GetWidgetFromName(TEXT("HP")));
	mMP= Cast<UTextBlock>(GetWidgetFromName(TEXT("MP")));
}

void USelectInfoWidget::SetCharacterImage(UMaterialInterface* Mtrl)
{
	mCharacterImage->SetBrushFromMaterial(Mtrl);
}

void USelectInfoWidget::SetCharacterInfo(ICharacterPublicInterface* Info)
{
	
	mJob->SetText(FText::FromString(Info->GetJobString()));

	mCharacterName->SetText(FText::FromString(Info->GetName()));

	FString Text = FString::Printf(TEXT("Power : %.2f"), Info->GetAttack());
	mAttack->SetText(FText::FromString(Text));

	Text = FString::Printf(TEXT("Def : %.2f"), Info->GetDefense());
	mDefense->SetText(FText::FromString(Text));

	Text = FString::Printf(TEXT("HP : %.2f"), Info->GetHP());
	mHP->SetText(FText::FromString(Text));

	Text = FString::Printf(TEXT("MP : %.2f"), Info->GetMP());
	mMP->SetText(FText::FromString(Text));
}

