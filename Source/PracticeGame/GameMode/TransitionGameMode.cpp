// Fill out your copyright notice in the Description page of Project Settings.


#include "TransitionGameMode.h"
#include "../PlayerController/OnTransitionPlayerController.h"

ATransitionGameMode::ATransitionGameMode()
{
	DefaultPawnClass = nullptr;// No default pawn for transition mode
	PlayerControllerClass = AOnTransitionPlayerController::StaticClass();
}


void ATransitionGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}

void ATransitionGameMode::InitGameState()
{
	Super::InitGameState();
}

void ATransitionGameMode::StartPlay()
{
	Super::StartPlay();
}

void ATransitionGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	if (!ErrorMessage.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("PreLogin failed: %s"), *ErrorMessage);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PreLogin succeeded for address: %s"), *Address);
	}
}
APlayerController* ATransitionGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* Result = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	//int32 �� �� ���ڸ� Job= ���Ŀ��� �Ľ��Ͽ�  Job ������ ����
	FParse::Value(*Options, TEXT("Job="), mSelectedJob);
	//FString Ÿ���� ���ڸ� MapName= ���Ŀ��� �Ľ��Ͽ� mNextMapName ������ ����
	FParse::Value(*Options, TEXT("MapName="), mNextMapName);
	//FString Ÿ���� ���ڸ� PlayerName= ���Ŀ��� �Ľ��Ͽ� mInputPlayerName ������ ����
	FParse::Value(*Options, TEXT("PlayerName="), mInputPlayerName);

	//Options ���ڿ����� MapPath= ������ ���� �Ľ��Ͽ� MapPath ������ ����
	FString MapPath;
	FParse::Value(*Options, TEXT("MapPath="), MapPath);

	UE_LOG(TestGame, Warning, TEXT("Job = %d"), mSelectedJob);
	UE_LOG(TestGame, Warning, TEXT("MapName = %s"), *mNextMapName);
	UE_LOG(TestGame, Warning, TEXT("MapPath= %s"), *MapPath);
	UE_LOG(TestGame, Warning, TEXT("PlayerName = %s"), *mInputPlayerName);
	

	//�񵿱�� ���� ������ �ε常 �Ѵ� �� ��쿡�� three param delegate �� ����Ѵ� 
	//FLoadPackageAsyncOptionalParams Params;

	/*
	PKG_ContainsMap		�� ��Ű���� ������ �����ϰ� �ִٴ� �ǹ�
	PKG_CompiledIn		�ڵ�� �����ϵǾ� ���Ե� ��Ű�� (C++ Ŭ���� ��)
	PKG_Cooked			��ŷ�� ������ ��Ű��
	PKG_EditorOnly		�����Ϳ����� �ʿ��� ��Ű�� -> Runtime ��� X 
	PKG_ContainsNoAsset	������ ���� ���
	PKG_UncookedOnly	���� ����. ��ŷ�� ���忡�� */

	LoadPackageAsync(MapPath,
		FLoadPackageAsyncDelegate::CreateUObject(this,			&ATransitionGameMode::LoadingAsyncComplete),
		0,
		PKG_ContainsMap);

	return Result;
}
void ATransitionGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("PostLogin called for player: %s"), *NewPlayer->GetName());
}

void ATransitionGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ATransitionGameMode::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("TransitionGameMode BeginPlay called"));
	
	// Additional initialization logic can be added here
}

void ATransitionGameMode::LoadingAsyncComplete(const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
{
	if (Result == EAsyncLoadingResult::Succeeded)
	{
		UE_LOG(TestGame, Warning, TEXT("LoadingAsyncComplete: Package %s loaded successfully."), *PackageName.ToString());
		FString Option = FString::Printf(TEXT("Job=%d PlayerName=%s"), (int32)mSelectedJob, *mInputPlayerName);

		UGameplayStatics::OpenLevel(GetWorld(), *mNextMapName, true, Option);
	}
}
