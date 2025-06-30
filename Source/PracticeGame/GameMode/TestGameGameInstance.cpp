// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameGameInstance.h"
#include "../UI/WidgetManager.h"
#include "../Data/Item/ItemDataCacheManager.h"
#include "../Data/Quest/QuestDataCacheManager.h"



	UTestGameGameInstance::UTestGameGameInstance()
	{
	}

	void UTestGameGameInstance::Init()
	{
		Super::Init();
	}

	void UTestGameGameInstance::Shutdown()
	{
		Super::Shutdown();

		//비상속 싱글턴 제거 
		CWidgetManager::DestroyInst();

		//싱글턴 UObject 상속 클래스들 제거
		UItemDataCacheManager::DestroyInst();
		UQuestDataCacheManager::DestroyInst();
	}