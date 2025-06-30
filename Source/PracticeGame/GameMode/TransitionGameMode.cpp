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

	//int32 로 된 문자를 Job= 이후에서 파싱하여  Job 변수에 저장
	FParse::Value(*Options, TEXT("Job="), mSelectedJob);
	//FString 타입의 문자를 MapName= 이후에서 파싱하여 mNextMapName 변수에 저장
	FParse::Value(*Options, TEXT("MapName="), mNextMapName);
	//FString 타입의 문자를 PlayerName= 이후에서 파싱하여 mInputPlayerName 변수에 저장
	FParse::Value(*Options, TEXT("PlayerName="), mInputPlayerName);

	//Options 문자열에서 MapPath= 이후의 값을 파싱하여 MapPath 변수에 저장
	FString MapPath;
	FParse::Value(*Options, TEXT("MapPath="), MapPath);

	UE_LOG(TestGame, Warning, TEXT("Job = %d"), mSelectedJob);
	UE_LOG(TestGame, Warning, TEXT("MapName = %s"), *mNextMapName);
	UE_LOG(TestGame, Warning, TEXT("MapPath= %s"), *MapPath);
	UE_LOG(TestGame, Warning, TEXT("PlayerName = %s"), *mInputPlayerName);
	

	//비동기로 다음 레벨을 로드만 한다 이 경우에는 three param delegate 를 사용한다 
	//FLoadPackageAsyncOptionalParams Params;

	/*
	PKG_ContainsMap		이 패키지가 레벨을 포함하고 있다는 의미
	PKG_CompiledIn		코드로 컴파일되어 포함된 패키지 (C++ 클래스 등)
	PKG_Cooked			쿠킹된 상태의 패키지
	PKG_EditorOnly		에디터에서만 필요한 패키지 -> Runtime 사용 X 
	PKG_ContainsNoAsset	에셋이 없을 경우
	PKG_UncookedOnly	개발 전용. 쿠킹된 빌드에서 */

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
