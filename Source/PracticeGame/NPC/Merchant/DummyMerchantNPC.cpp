// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyMerchantNPC.h"
#include "../NPCData.h"


ADummyMerchantNPC::ADummyMerchantNPC()
{
	mDataKey = TEXT("MerchantDummy");
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonGRIMexe/Characters/Heroes/GRIM/Skins/Tier1/Razorred/Meshes/GRIMRazorRed.GRIMRazorRed'"));

	if (SkeletalMesh.Succeeded())
		mMesh->SetSkeletalMeshAsset(SkeletalMesh.Object);

	mBody->SetCapsuleHalfHeight(134.0f);
	mBody->SetCapsuleRadius(64.425728f);

	mMesh->SetRelativeLocation(FVector(0, 0, -134.0f));

}

void ADummyMerchantNPC::OnClicked()
{
	Super::OnClicked();
}

void ADummyMerchantNPC::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	const UNPCData* NPCData =
		GetDefault<UNPCData>();

	if (NPCData)
	{
		FNPCTableInfo* TableInfo =
			NPCData->FindNPCInfo(mDataKey);

		if (TableInfo)
		{
			//mName = TableInfo->Name;
			//mAttack = TableInfo->Attack;
			//mDefense = TableInfo->Defense;
			//mHP = TableInfo->HP;
			//mHPMax = TableInfo->HP;
			//mMP = TableInfo->MP;
			//mMPMax = TableInfo->MP;
			//mAttackDistance = TableInfo->AttackDistance;
			//mMoveSpeed = TableInfo->MoveSpeed;
			//mDetectDistance = TableInfo->DetectDistance;
			//mLevel = TableInfo->Level;
			//mExp = TableInfo->Exp;
			//mGold = TableInfo->Gold;

			//mMovement->SetMaxSpeed(mMoveSpeed);

			UE_LOG(TestGame, Warning, TEXT("Monster Info"));
		}
	}
}
