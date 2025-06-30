// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../GameInfo.h"
#include "InputActionValue.h"
#include "../PlayerController/PlayerControllerTestGame.h"
#include "../Interface/CharacterPublicInterface.h"
#include "../Interface/CharacterRenderingInterface.h"
#include "../Effects/AfterImage.h"
#include "../Actor/ATestProjectile.h"
#include "InventoryComponent.h"
#include "QuestComponent.h"

#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

//플레이어캐릭터 관련 로그설정
DECLARE_LOG_CATEGORY_EXTERN(PlayerLog, Log, All);

UENUM(BlueprintType)
enum class ETrailAssetMode : uint8
{
	NIAGARASYSTEM,
	PARTICLESYSTEM
};

UCLASS()	//Blueprintable = 블루 프린트에서 확장 가능
class PRACTICEGAME_API APlayerCharacter
	: public ACharacter,
	public IWeaponTrailEffectProvider,
	public ICharacterPublicInterface,
	public ICharacterRenderingInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere)
	bool bNormalAttackOverlap = true;

protected:
	//디버그 메시지 출력 토글
	UPROPERTY(EditAnywhere)
	bool bDebugEnabled = false;

	//스프링 암을 가지고 있을 멤버를 선언한다
	//에디터에서 수정이 가능하다
	UPROPERTY(EditAnywhere)
	TObjectPtr<USpringArmComponent> mSpringArm;

	//카메라 컴포넌트를 가지고 있을  멤버를 선언한다
	//게임 실행 중에 볼 수 있다
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent>	mCamera;

	//애님인스턴스를 받아둘 포인터
	TObjectPtr<class UPlayerTemplateAnimInstance>	mAnimInst;

	//적/아군/중립을 판별하기 위한 정보
	FGenericTeamId	mTeamID;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AATestProjectile> mProjectileClass;

	//케릭터 상태 관리를 위한 구조체이다  비트플래그로  비트연산자를 지원한다
	//8비트이기 때문에 최대 8개의 상태를 저장할 수 있다
	FCharacterState	mCharacterState;

	//무기의 파티클 시스템이 무엇인지 구별한다
	UPROPERTY(EditAnywhere)
	ETrailAssetMode  mTrailAssetMode = ETrailAssetMode::PARTICLESYSTEM;
	
	//타이머 관련 기능을 조작할 핸들러이다
	FTimerHandle					mDashTimer;
public:
	FTimerHandle					mDashAfterImageTimerHandler;

protected:		
	bool			bIsRightMouseButtonPressed = false;

	//타깃지면을 지정하는 스킬이 활성화 중인지를 체크하는 멤버
	bool			mSelectingCastZone = false;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USoundWave>	mSfxDash;
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TArray<UBoxComponent*> mWeaponColBox;

protected:
	//왼손 오른손 무기 트레일 파티클
	TObjectPtr<UParticleSystem>		mTrailEffectL;
	TObjectPtr<UParticleSystem>		mTrailEffectR;

	//왼손 오른손 무기 트레일 파티클 (나이아가라인 경우)
	TObjectPtr<UNiagaraSystem>		mTrailEffectNiagaraL;
	TObjectPtr<UNiagaraSystem>		mTrailEffectNiagaraR;

	//임시영역 스텐실 - 어클루전 처리 시 이전 대상들을 받아두기 위함
	TArray<ICharacterRenderingInterface*>		mPrevDetectedActors;
	
	UPROPERTY(EditAnywhere)
	FName	mDataKey;

	TObjectPtr<UInventoryComponent>		mInventoryItemsComponent;
	//스킬컴포넌트
	TObjectPtr<UQuestComponent>		mQuestComponent;

public:
	UInventoryComponent* GetInventoryComponent() const
	{
		return mInventoryItemsComponent;
	}
	UQuestComponent* GetQuestComponent() const
	{
		return mQuestComponent;
	}
	
public:
	UpdateHealthDelegate	mOnHealthChange;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void OnConstruction(const FTransform& Transform);

private:
	//입력 액션에 의해 바인딩할 함수는
	// 반드시 void 반환타입
	// const FInputActionValue& Value 인자 타입을 설정해야 한다
	void OnIAMoveTriggered(const FInputActionValue& Value);					//1.이동
	void OnIAJumpStarted(const FInputActionValue& Value);					//2.점프
	void OnIAAttackStarted(const FInputActionValue& Value);					//3. 공격
	void OnIAAttackTriggered(const FInputActionValue& Value);				//3b.공격
	void OnIACamRotateTriggered(const FInputActionValue& Value);			//4. 카메라 로테이션
	void OnIACharacterRotateTriggered(const FInputActionValue& Value);		//4b. + 캐릭터 로테이션
	void OnIACharacterRotateCompleted(const FInputActionValue& Value);		//4c. + 캐릭터 로테익션 종료
	void OnIADashStarted(const FInputActionValue& Value);					//5. 대시
	void OnIASprintTriggered(const FInputActionValue& Value);				//6. 스프린트 시작
	void OnIASprintCompleted(const FInputActionValue& Value);				//6b. 스프린트 종료
	void OnIASpecialStarted(const FInputActionValue& Value);				//7. 스페셜능력
	void OnIAPreviewThanCastStarted(const FInputActionValue& Value);		//8. 위치지정스킬 준비
	void OnIAAcceptCastStarted(const FInputActionValue& Value);		//8b. 위치지정스킬 수락
	void OnIACancelCastStarted(const FInputActionValue& Value);		//8c. 위치지정스킬 취소

	void OnIACleaveStarted(const FInputActionValue& Value);					//8. 부채꼴베기(클리브)

	//UI 패널 단축키 바인딩
	void OnIAPlayerInfoPanelStarted(const FInputActionValue& Value);		//9. 플레이어 정보 패널
	void OnIASelectCharacterStarted(const FInputActionValue& Value);		//10. 엑터 선택 액션

	void OnIAPanelSkillStarted(const FInputActionValue& Value);		//9. 플레이어 정보 패널
	void OnIAPanelInvStarted(const FInputActionValue& Value);		//10. 엑터 선택 액션
protected:
	//케릭터를 Axis 벡터로 ScaleValue 강도로 이동 시킨다.
	//캐릭터 기본 이동과 관련된 부가적인 연산을 수행할 수 있다
	void MoveToAxis(const FVector& Axis, const float ScaleValue = 1.0f);
	
public:
	virtual void NormalAttack();  //노티파이에서 호출하기 위해 퍼블릭:
protected:
	virtual void SpecialAttack();
	virtual void CleaveAttack();

	void Dash(float DashSpeed);
	void RestoreGravity();
	void Sprint();
	void StopSprint();

	virtual void MakePreviewCastZone();
	virtual void CancelPreviewCastZone();
	virtual void AcceptPreviewCastZone();
	virtual void InitCastZoneSkill();

	protected:
		void AfterImageLoop();

	/*Cone 모양의 스윕 트레이스를 시행하고 영역안의 몬스터클래스들에 한해서 데미지를 계산한다
	* 모든 충돌에 파티클과 사운드를 스폰할지 결정할 수 있다.
	* VFX, SFX 에셋을 전달하지 않으면 스폰하지 않는다
	* bSpawnVFXEveryHit 을 false 로 전달하면 캐릭터 location 에 VFX 를 한 번 스폰한다
	* bSpawnSFXEveryHit 을 false 로 전달하면 캐릭터 location 에 SFX 를 한 번 스폰한다
	*/
	virtual void RunSweepConeAndDamage(float Damage, float SphereSize, float SphereDistance, float AngleStartDistance, float InnerAngle, ECollisionChannel TraceChannel, UNiagaraSystem* VFXAsset = nullptr, bool bSpawnVFXOnHit = true, USoundBase* SFXAsset = nullptr, bool bSpawnSFXOnHit = true);
	//사운드 에셋 다른 오버로딩 함수
	virtual void RunSweepConeAndDamage(float Damage, float SphereSize, float SphereDistance, float AngleStartDistance, float InnerAngle, ECollisionChannel TraceChannel, UParticleSystem* VFXAsset = nullptr, bool bSpawnVFXOnHit = true, USoundBase* SFXAsset = nullptr, bool bSpawnSFXOnHit = true);

public:
	void SetPreviewCastActivation(bool NewSetup)
	{
		if(NewSetup != mSelectingCastZone)
			mSelectingCastZone = NewSetup;
	}
	bool GetPreviewCastActivation()
	{
		return mSelectingCastZone;
	}
	//이동불능 상태인지 확인한다, 이 상태에서는 이동 관련 행위를 못하도록 설계한다
	virtual bool IsCharacterImmobilized();
	//공격과 같은 주요 액션 행위가 가능한 상태인지 반환한다
	virtual bool CanCharacterAct();
	//케릭터상태 구조체를 반환한다

public:
	FTimerHandle GetDashAfterImageTimerHandler()
	{
		return mDashAfterImageTimerHandler;
	}

	TArray<UBoxComponent*> GetWeaponColBox()
	{
		return mWeaponColBox;
	}

	FCharacterState& GetCharacterState()
	{
		return mCharacterState;
	}

	FCharacterState* GetCharacterStatePtr()
	{
		return &mCharacterState;
	}

	TSubclassOf<AATestProjectile> GetProjectileClass() const
	{
		return mProjectileClass;
	}

	ETrailAssetMode GetTrailAssetMode() const
	{ 
		return mTrailAssetMode; 
	}

	virtual UParticleSystem* GetLeftWeaponTrailEffectParticle() const override 
	{ 
		return mTrailEffectL.Get(); 
	}
	virtual UNiagaraSystem* GetLeftWeaponTrailEffectNiagara() const override 
	{ 
		return mTrailEffectNiagaraL.Get(); 
	}

	virtual UParticleSystem* GetRightWeaponTrailEffectParticle() const override 
	{
		return mTrailEffectR.Get(); 
	}
	virtual UNiagaraSystem* GetRightWeaponTrailEffectNiagara() const override 
	{
		return mTrailEffectNiagaraR.Get(); 
	}

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//모든 컴포넌트가 초기화된 후 호출된다
	virtual void PostInitializeComponents() override;
	/*Called upon landing when falling, to perform actions based on the Hit result.Triggers the OnLanded event.
		* Note that movement mode is still "Falling" during this event.Current Velocity value is the velocity at the time of landing.*/
	virtual void Landed(const FHitResult& Hit) override;
	/** Called when the character's movement enters falling */
	virtual void Falling() override;
	/** Called when character's jump reaches Apex. Needs CharacterMovement->bNotifyApex = true */
	virtual void NotifyJumpApex() override;

public:
	void ApplyTrailEffect(bool bLeftHand, USkeletalMeshComponent* MeshComp, int32 CurrentTrackIndex);

protected:
		//무기 충돌체의 오버렙 이벤트 시 호출할 델리게이트 함수를 정의한다
	UFUNCTION()
	void OnWeaponOverlap(
			UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,	FVector NormalImpulse,	const FHitResult& HitResult);

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);

public:
	virtual void SetGenericTeamId(const FGenericTeamId& TeamID);
	virtual FGenericTeamId GetGenericTeamId() const;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const;
	
	//스텐실 효과를 켜고 끈다
	virtual void EnableOutLine(bool Enable);
	virtual void EnableOcclusion(bool Enable);

	virtual void UpdateHPBar();
	virtual void UpdateMPBar();

	virtual void OnGetExp(int32 AddNumber);
	virtual void LevelUp();

	virtual void GenerateRandomInvItem();
};

