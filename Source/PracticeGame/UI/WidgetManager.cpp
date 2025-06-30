// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetManager.h"

//�̱��� mInst �ʱ�ȭ
CWidgetManager* CWidgetManager::mInst = nullptr;

CWidgetManager::CWidgetManager()
{
}

CWidgetManager::~CWidgetManager()
{
}

void CWidgetManager::AddWidget(const FString& Name, UUserWidgetBase* Widget)
{
	//�̹� �ش� ������ �ִ°�� �߰����� �ʴ´�
	if(mWidgetMap.Contains(Name))
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget with name %s already exists."), *Name);
		return;
	}

	//������ ��� TMap�� �߰��Ѵ�
	mWidgetMap.Add(Name, Widget);
}

void CWidgetManager::DeleteWidget(const FString& Name)
{
	//�ش� ������ ���°�� ���Ÿ� �õ����� �ʴ´�
	if (!mWidgetMap.Contains(Name))
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget with name %s not exists."), *Name);
		return;
	}

	//������ ��� TMap���� �����Ѵ�
	mWidgetMap.Remove(Name);
}