// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultWeaponTrails.h"

//Ʈ���� ��Ƽ���� ���� �� ȣ��
void UDefaultWeaponTrails::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)

{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	// ĳ���� Ŭ�������� ��ƼŬ�� ��������
	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid OwnerCharacter!"));
		return;
	}
	
	// ��Ƽ���� �̺�Ʈ���� Ʈ���� �� ����
	const FAnimNotifyEvent* NotifyEvent = EventReference.GetNotify();
	bool bLeftHand = (NotifyEvent && NotifyEvent->TrackIndex == 1); //TEXT("TrailEffectL"));
	
	if (NotifyEvent && bDebugMode )
	{
		UE_LOG(LogTemp, Warning, TEXT("Notify Name: [%d] %s"), NotifyEvent->TrackIndex, *NotifyEvent->NotifyName.ToString());
	}
	else if(!NotifyEvent && bDebugMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("NotifyEvent is nullptr!"));
	}

	//Ʈ���� ����
	OwnerCharacter->ApplyTrailEffect(bLeftHand, MeshComp, NotifyEvent->TrackIndex);


	//�� �ڵ� (�������̽� ��� ��)
	//// Ʈ������ ������ �� ���� ����
	//const FAnimNotifyEvent* NotifyEvent = EventReference.GetNotify();
	//FName SelectedSocketName = (NotifyEvent && NotifyEvent->NotifyName == TEXT("TrailEffectL"))
	//	? FName("weapon_l_Trail")
	//	: FName("weapon_r_Trail");

	//UNiagaraSystem* SelectedTrailNiagara = (NotifyEvent && NotifyEvent->NotifyName == TEXT("TrailEffectL"))
	//	? OwnerCharacter->GetLeftWeaponTrailEffectNiagara()
	//	: OwnerCharacter->GetRightWeaponTrailEffectNiagara();

	//UParticleSystem* SelectedTrailParticle = (NotifyEvent && NotifyEvent->NotifyName == TEXT("TrailEffectL"))
	//	? OwnerCharacter->GetLeftWeaponTrailEffectParticle()
	//	: OwnerCharacter->GetRightWeaponTrailEffectParticle();
	//
	//if (OwnerCharacter->GetTrailAssetMode() == ETrailAssetMode::NIAGARASYSTEM)
	//{
	//	//Ʈ���� ����Ʈ�� �����ϰ� ���Ͽ� �����Ѵ�
	//	UNiagaraComponent* TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
	//		SelectedTrailNiagara,
	//		MeshComp,
	//		SelectedSocketName,
	//		FVector::ZeroVector,
	//		FRotator::ZeroRotator,
	//		EAttachLocation::KeepRelativeOffset,
	//		true
	//	);
	//	//Ʈ���� ���ӽð� ��������Ǽ��� (�ʿ��)
	//	if (TrailComponent)
	//	{
	//		TrailComponent->SetFloatParameter(TEXT("TrailDuration"), TotalDuration);
	//	}
	//}
	//if (OwnerCharacter->GetTrailAssetMode() == ETrailAssetMode::PARTICLESYSTEM)
	//{
	//	//Ʈ���� ����Ʈ�� �����ϰ� ���Ͽ� �����Ѵ�
	//	UParticleSystemComponent* TrailComponent
	//		= UGameplayStatics::SpawnEmitterAttached(
	//			SelectedTrailParticle,
	//			MeshComp,
	//			SelectedSocketName,
	//			FVector::ZeroVector,
	//			FRotator::ZeroRotator,
	//			EAttachLocation::KeepRelativeOffset
	//		);
	//	//Ʈ���� ���ӽð� ��������Ǽ��� (�ʿ��)
	//	if (TrailComponent)
	//	{
	//		TrailComponent->SetFloatParameter(TEXT("TrailDuration"), TotalDuration);
	//	}
	//}

	//���� Ŭ������ �̺�Ʈ ó���帧�� �����ϸ鼭
	//�߰����� ��������� ������ �����ϱ� ���� �����Ѵ�
	//Received_NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

//Ʈ���� ��Ƽ���� ���� �� ȣ��
void UDefaultWeaponTrails::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	//if (!IsValid(GetWorld()))
	//{
	//	// �����ϰ� ���� �Ǵ� ��ü ó��
	//	return;
	//}
	//�޽�������Ʈ ���� ���� �˻�, ���������� �������� �ʵ��� üũ
	if (!MeshComp || MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	//�ش� ���Ͽ� ����� Ʈ������ ã�� �����Ѵ�
	TArray<USceneComponent*> AttachedComponents;
	//���� �޽��� �ִ� �ڼ�������Ʈ�� �ּҸ� �迭�� �޾ƿ´�
	MeshComp->GetChildrenComponents(true, AttachedComponents);

	//��Ƽ���̿��� Ʈ�������� �޾ƿ´� 
	// ���� ��Ƽ���� Ŭ������ ���� ������ 
	// Ʈ���� �������� ������ ���ӽð��� �����ִ� �ٸ� Ʈ���� ��Ƽ���̱��� ��� ���Ű��ȴ�.

	const FAnimNotifyEvent* NotifyEvent = EventReference.GetNotify();
	if (!NotifyEvent && bDebugMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("NotifyEvent is nullptr!"));
		return;
	}

	int32 TargetTrackIndex = NotifyEvent->TrackIndex; // �ش� Ʈ�� �ε��� ��������
	if(bDebugMode)
		UE_LOG(LogTemp, Warning, TEXT("Ending Notify on Track: %d"), TargetTrackIndex);

	// ����� Ʈ������ ã�Ƽ� ���� (Ư�� Ʈ����)
	for (USceneComponent* Component : AttachedComponents)
	{
		// Particle System Ʈ���� ���� (Ʈ�� ���͸� �߰�)
		if (UParticleSystemComponent* ParticleComp = Cast<UParticleSystemComponent>(Component))
		{
			if (ParticleComp->IsActive() && ParticleComp->ComponentTags.Contains(FName(*FString::FromInt(TargetTrackIndex))))
			{
				ParticleComp->Deactivate();
				ParticleComp->DestroyComponent();
				if(bDebugMode)
					UE_LOG(LogTemp, Warning, TEXT("Destroy Notifies on Target Track Index: %d"), TargetTrackIndex);
			}
		}

		// Niagara System Ʈ���� ���� (Ʈ�� ���͸� �߰�)
		if (UNiagaraComponent* NiagaraComp = Cast<UNiagaraComponent>(Component))
		{
			if (NiagaraComp->IsActive() && NiagaraComp->ComponentTags.Contains(FName(*FString::FromInt(TargetTrackIndex))))
			{
				NiagaraComp->Deactivate();
				NiagaraComp->DestroyComponent();
				if(bDebugMode)
					UE_LOG(LogTemp, Warning, TEXT("Destroy Notifies on Target Track Index: %d"), TargetTrackIndex);
			}
		}
	}
}



