// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"

//싱글턴 mInst 초기화
CWidgetManager* CWidgetManager::mInst = nullptr;

CWidgetManager::CWidgetManager()
{
}

CWidgetManager::~CWidgetManager()
{
}

void CWidgetManager::AddWidget(const FString& Name, UUserWidgetBase* Widget)
{
	//이미 해당 위젯이 있는경우 추가하지 않는다
	if(mWidgetMap.Contains(Name))
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget with name %s already exists."), *Name);
		return;
	}

	//위젯을 멤버 TMap에 추가한다
	mWidgetMap.Add(Name, Widget);
}

void CWidgetManager::DeleteWidget(const FString& Name)
{
	//해당 위젯이 없는경우 제거를 시도하지 않는다
	if (!mWidgetMap.Contains(Name))
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget with name %s not exists."), *Name);
		return;
	}

	//위젯을 멤버 TMap에서 제거한다
	mWidgetMap.Remove(Name);
}