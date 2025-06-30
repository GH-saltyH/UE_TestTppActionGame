// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectPawn.h"
#include "PlayerData.h"


// Sets default values
ASelectPawn::ASelectPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	mMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));

	SetRootComponent(mBody);

	mMesh->SetupAttachment(mBody);
	mMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	mBody->SetCollisionProfileName(TEXT("Player"));
}

void ASelectPawn::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	UE_LOG(TestGame, Warning, TEXT("SelectPawn : OnConstruction Entered"));

	//const UPlayerData* PlayerData =
	//	GetDefault<UPlayerData>();

	//if (PlayerData)
	//{
	//	FPlayerTableInfo* TableInfo =
	//		PlayerData->FindPlayerInfo(mDataKey);

	//	if (TableInfo)
	//	{
	//		mJob = TableInfo->Job;
	//		
	//		//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerJob"), true);
	//		//FString JobString;

	//		//if (!EnumPtr) 
	//		//	JobString = FString("Invalid");
	//		//else
	//		//	JobString = EnumPtr->GetNameStringByValue((int64)TableInfo->Job);

	//		/*UE_LOG(TestGame, Warning, 
	//			TEXT("mJob Code [%d], GetJobString [%s]"), 
	//			TableInfo->Job, 
	//			*JobString);*/
	//		mName = TableInfo->Name;
	//		//UE_LOG(TestGame, Warning, TEXT("mName = [%s]"), *mName);
	//		mAttack = TableInfo->Attack;
	//		mDefense = TableInfo->Defense;
	//		mHP = TableInfo->HP;
	//		mHPMax = TableInfo->HP;
	//		mMP = TableInfo->MP;
	//		mMPMax = TableInfo->MP;
	//		mAttackDistance = TableInfo->AttackDistance;
	//		mMoveSpeed = TableInfo->MoveSpeed;
	//		mLevel = TableInfo->Level;
	//		mExp = TableInfo->Exp;
	//		mGold = TableInfo->Gold;

	//		//mMovement->SetMaxSpeed(mMoveSpeed);

	//		UE_LOG(TestGame, Warning, TEXT("Player Info"));
	//	}
	//}
}

// Called when the game starts or when spawned
void ASelectPawn::BeginPlay()
{
	Super::BeginPlay();
	
	mAnimInst = Cast<USelectPawnAnimInstanceTemplate>(mMesh->GetAnimInstance());

	const UPlayerData* PlayerData =
		GetDefault<UPlayerData>();

	if (PlayerData)
	{
		FPlayerTableInfo* TableInfo =
			PlayerData->FindPlayerInfo(mDataKey);

		if (TableInfo)
		{
			mJob = TableInfo->Job;

			//const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EPlayerJob"), true);
			//FString JobString;

			//if (!EnumPtr) 
			//	JobString = FString("Invalid");
			//else
			//	JobString = EnumPtr->GetNameStringByValue((int64)TableInfo->Job);

			/*UE_LOG(TestGame, Warning,
				TEXT("mJob Code [%d], GetJobString [%s]"),
				TableInfo->Job,
				*JobString);*/
			mName = TableInfo->Name;
			//UE_LOG(TestGame, Warning, TEXT("mName = [%s]"), *mName);
			mAttack = TableInfo->Attack;
			mDefense = TableInfo->Defense;
			mHP = TableInfo->HP;
			mHPMax = TableInfo->HP;
			mMP = TableInfo->MP;
			mMPMax = TableInfo->MP;
			mAttackDistance = TableInfo->AttackDistance;
			mMoveSpeed = TableInfo->MoveSpeed;
			mLevel = TableInfo->Level;
			mExp = TableInfo->Exp;
			mGold = TableInfo->Gold;

			//mMovement->SetMaxSpeed(mMoveSpeed);

			UE_LOG(TestGame, Warning, TEXT("Player Info"));
		}
	}
}

// Called every frame
void ASelectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASelectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


void ASelectPawn::EnableMeshStencil(bool Enable)
{
	mMesh->SetRenderCustomDepth(Enable);
}


void ASelectPawn::ChangeAnim(ESelectAnim Anim)
{
	if (IsValid(mAnimInst))
		mAnimInst->ChangeAnim(Anim);
}