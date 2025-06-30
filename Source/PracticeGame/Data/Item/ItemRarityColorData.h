// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../GameInfo.h"
#include "UObject/NoExportTypes.h"
#include "ItemRarityColorData.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PRACTICEGAME_API UItemRarityColorData : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FItemRarityColor> RarityColors;

	FLinearColor GetColorByRarity(EItemRarity Rarity)	const
	{
		for (const FItemRarityColor& Entry : RarityColors)
		{
			if (Entry.Rarity == Rarity)
				return Entry.Color;
		}
		return FLinearColor::White;
	}
	
};
