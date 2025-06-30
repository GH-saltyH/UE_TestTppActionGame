// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "UserWidgetBase.h"

/**
 * 
 */
class PRACTICEGAME_API CWidgetManager
{
private:
	CWidgetManager();
	~CWidgetManager();

private:
	static CWidgetManager* mInst;

private:
	TMap<FString, TObjectPtr<UUserWidgetBase>> mWidgetMap;

public:
	void AddWidget(const FString& Name, UUserWidgetBase* Widget);
	void DeleteWidget(const FString& Name);
	template <typename T>
	T* FindWidget(const FString& Name)
	{
		return Cast<T>(mWidgetMap.FindRef(Name));
	}

public:
	static CWidgetManager* GetInst()
	{
		if (!mInst)
			mInst = new CWidgetManager;

		return mInst;
	}

	static void DestroyInst()
	{
		if (mInst)
		{
			delete mInst;
			mInst = nullptr;
		}
	}
};
