// Fill out your copyright notice in the Description page of Project Settings.


#include "AfterImage.h"


// Sets default values
AAfterImage::AAfterImage()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	mRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	mMesh = CreateDefaultSubobject<UPoseableMeshComponent>(TEXT("Mesh"));

	SetRootComponent(mRoot);

	mMesh->SetupAttachment(mRoot);

	mMesh->SetRelativeRotation(FRotator(0, 0, 0));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> OriginMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/MT_Trans_Fresnel_Inst.MT_Trans_Fresnel_Inst'"));
	if (OriginMaterial.Succeeded())
		mOriginMaterial = OriginMaterial.Object;
}

void AAfterImage::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAfterImage::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mFadeTime -= DeltaTime / 1.5;

	if (mFadeTime <= 0.f)
	{
		Destroy(); mFadeTime = 0.f;
	}

	for (auto& Material : mMaterials)
	{
		Material->SetScalarParameterValue(TEXT("Opacity"), mFadeTime);
	}

}

void AAfterImage::CopyImage(USkeletalMeshComponent* Dest)
{
	/*'USkinnedMeshComponent::SkeletalMesh': Replaced by SkinnedAsset. Use GetSkinnedAsset()/SetSkinnedAsset() instead, or GetSkeletalMeshAsset/SetSkeletalMeshAsset() when called from a USkeletalMeshComponent. Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.*/

	//대상의 스켈레탈 메시로 
	mMesh->SetSkeletalMesh(Dest->SkeletalMesh);			//GetSkeletalMeshAsset());  
	

	//대상의 메시로부터 포즈를 복사하여 현재 메시에 받는다
	mMesh->CopyPoseFromSkeletalComponent(Dest);

	//1틱 지연 실행 -> 문제 있다면, 
	//GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AAfterImage::UpdateCopyMesh);
	
	//지연 없이 실행
	UpdateCopyMesh();

	// Dest Component의 Pose정보를 복제한 Mesh를 만들어낸다.
	//mMesh->CopyPoseFromSkeletalComponent(Dest);
	/*int32	BoneCount = Dest->GetNumBones();

	for (int32 i = 0; i < BoneCount; ++i)
	{
		FName	BoneName = Dest->GetBoneName(i);
		FTransform	BoneTransform = Dest->GetBoneTransform(i);
		mMesh->SetBoneTransformByName(BoneName, BoneTransform,
			EBoneSpaces::ComponentSpace);if
	}

	mMesh->RefreshBoneTransforms();*/

	/*int32	Count = Dest->GetNumMaterials();

	for (int32 i = 0; i < Count; ++i)
	{
		UMaterialInstanceDynamic* Mtrl = mMesh->CreateDynamicMaterialInstance(i,
			mOriginMaterial);

		Mtrl->BlendMode = EBlendMode::BLEND_Translucent;

		mMaterials.Add(Mtrl);
	}*/

	//UMaterialInstanceDynamic* Mtrl = UMaterialInstanceDynamic::Create(mOriginMaterial,
	//	this);

	//for (int32 i = 0; i < Count; ++i)
	//{
	//	mMesh->SetMaterial(i, Mtrl);
		/*UMaterialInstanceDynamic* Mtrl = mMesh->CreateDynamicMaterialInstance(i,
			mOriginMaterial);

		Mtrl->BlendMode = EBlendMode::BLEND_Translucent;

		mMaterials.Add(Mtrl);*/
		//}
}

void AAfterImage::SetOriginMaterial(const FString& Path)
{
	mOriginMaterial = LoadObject<UMaterialInterface>(nullptr, *Path);
}

void AAfterImage::SetOriginMaterial(UMaterialInterface* Material)
{
	mOriginMaterial = Material;
}

void AAfterImage::UpdateCopyMesh()
{
	int32	Count = mMesh->GetNumMaterials();

	for (int32 i = 0; i < Count; ++i)
	{
		UMaterialInstanceDynamic* Material = mMesh->CreateDynamicMaterialInstance(i, mOriginMaterial);

		Material->BlendMode = EBlendMode::BLEND_Translucent;

		mMaterials.Add(Material);
	}
}

