// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultWeaponTrails.h"

//트레일 노티파이 시작 시 호출
void UDefaultWeaponTrails::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference)

{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (!MeshComp) return;

	// 캐릭터 클래스에서 파티클을 가져오기
	APlayerCharacter* OwnerCharacter = Cast<APlayerCharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid OwnerCharacter!"));
		return;
	}
	
	// 노티파이 이벤트에서 트레일 손 결정
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

	//트레일 적용
	OwnerCharacter->ApplyTrailEffect(bLeftHand, MeshComp, NotifyEvent->TrackIndex);


	//구 코드 (인터페이스 사용 전)
	//// 트레일을 적용할 본 소켓 결정
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
	//	//트레일 이펙트를 생성하고 소켓에 부착한다
	//	UNiagaraComponent* TrailComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
	//		SelectedTrailNiagara,
	//		MeshComp,
	//		SelectedSocketName,
	//		FVector::ZeroVector,
	//		FRotator::ZeroRotator,
	//		EAttachLocation::KeepRelativeOffset,
	//		true
	//	);
	//	//트레일 지속시간 사용자정의설정 (필요시)
	//	if (TrailComponent)
	//	{
	//		TrailComponent->SetFloatParameter(TEXT("TrailDuration"), TotalDuration);
	//	}
	//}
	//if (OwnerCharacter->GetTrailAssetMode() == ETrailAssetMode::PARTICLESYSTEM)
	//{
	//	//트레일 이펙트를 생성하고 소켓에 부착한다
	//	UParticleSystemComponent* TrailComponent
	//		= UGameplayStatics::SpawnEmitterAttached(
	//			SelectedTrailParticle,
	//			MeshComp,
	//			SelectedSocketName,
	//			FVector::ZeroVector,
	//			FRotator::ZeroRotator,
	//			EAttachLocation::KeepRelativeOffset
	//		);
	//	//트레일 지속시간 사용자정의설정 (필요시)
	//	if (TrailComponent)
	//	{
	//		TrailComponent->SetFloatParameter(TEXT("TrailDuration"), TotalDuration);
	//	}
	//}

	//상위 클래스의 이벤트 처리흐름을 유지하면서
	//추가적인 사용자정의 로직을 실행하기 위해 설정한다
	//Received_NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
}

//트레일 노티파이 종료 시 호출
void UDefaultWeaponTrails::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	//if (!IsValid(GetWorld()))
	//{
	//	// 안전하게 리턴 또는 대체 처리
	//	return;
	//}
	//메쉬컴포넌트 존재 여부 검사, 서버에서는 실행하지 않도록 체크
	if (!MeshComp || MeshComp->GetWorld()->GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	//해당 소켓에 연결된 트레일을 찾고 제거한다
	TArray<USceneComponent*> AttachedComponents;
	//현재 메쉬에 있는 자손컴포넌트의 주소를 배열로 받아온다
	MeshComp->GetChildrenComponents(true, AttachedComponents);

	//노티파이에서 트랙정보를 받아온다 
	// 같은 노티파이 클래스를 쓰기 때문에 
	// 트랙을 구분하지 않으면 지속시간이 남아있는 다른 트랙의 노티파이까지 모두 제거가된다.

	const FAnimNotifyEvent* NotifyEvent = EventReference.GetNotify();
	if (!NotifyEvent && bDebugMode)
	{
		UE_LOG(LogTemp, Warning, TEXT("NotifyEvent is nullptr!"));
		return;
	}

	int32 TargetTrackIndex = NotifyEvent->TrackIndex; // 해당 트랙 인덱스 가져오기
	if(bDebugMode)
		UE_LOG(LogTemp, Warning, TEXT("Ending Notify on Track: %d"), TargetTrackIndex);

	// 연결된 트레일을 찾아서 제거 (특정 트랙만)
	for (USceneComponent* Component : AttachedComponents)
	{
		// Particle System 트레일 해제 (트랙 필터링 추가)
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

		// Niagara System 트레일 해제 (트랙 필터링 추가)
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



