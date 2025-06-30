// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine.h"
#include "Engine/OverlapResult.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "Animation/AimOffsetBlendSpace.h"

#include "NiagaraFunctionLibrary.h"			//나이아가라기능라이브러리
#include "NiagaraComponent.h"					//나이아가라 사용을 위해 필요
#include "Engine/DataTable.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Kismet/KismetMathLibrary.h"

//미디어컴포넌트 / 시네마틱 관련
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "MediaPlayer.h"
#include "FileMediaSource.h"

#include "GameInfo.generated.h"

#define	TeamNeutral	255
#define	TeamNPC		50
#define	TeamMonster	30
#define	TeamPlayer	10

//TestGame 관련 로그설정
DECLARE_LOG_CATEGORY_EXTERN(TestGame, Log, All);

DECLARE_MULTICAST_DELEGATE_TwoParams(UpdateHealthDelegate, float, float);

//자신과 대상의 차이를 고려하여 타깃으로 향하는 회전값을 구한다
FRotator GetTargetRotator(const FVector& Target, const FVector& Current);
//같은 목적의 Yaw 회전을 구한다
FRotator GetTargetRotatorYaw(FVector Target, FVector Current);


//재생할 애니메이션을 결정하기 위해 사용하는 열거형이다
UENUM(BlueprintType)
enum class EPlayerAnim : uint8	
{
	Ground,
	Jump,
	Falling,
	MoveXY,
	Landed,
    Dash,
    Special,
    Cleave
};

UENUM()
enum class EPlayerJob : uint8
{
    None,
    Rogue,
    Hunter,
    Fighter
};

//케릭터 상태정보를 저장하는 구조체
struct FCharacterState
{
    uint8 StateFlags;  // 최대 8개의 상태 저장 가능 (8bit)

    enum ECharacterFlags
    {
        STUNNED              = 1 << 0,           // 00000001
        IMMOBILIZED         = 1 << 1,           // 00000010
        INVULNERABLE      = 1 << 2,           // 00000100
        ACTING                 = 1 << 3,            // 00001000
        LANDING               = 1 << 4,            // 00010000
        COMBO                 = 1 << 5              // 00100000
    };

    //해당 상태를 갖고 있는지=true 반환한다
    bool HasState(uint8 Flag) const
    {
        return (StateFlags & Flag) != 0;
    }

    //상태를 설정한다 -> 1
    void SetState(uint8 Flag)
    {
        StateFlags |= Flag;
    }

    //상태를 해제한다 -> 0
    void ClearState(uint8 Flag)
    {
        StateFlags &= ~Flag;
    }
};

USTRUCT(BlueprintType)
struct FLevelExpRow : public FTableRowBase
{
    GENERATED_BODY()
    
    //레벨 정보
    UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Level")
	int32 Level = 1;           
    //요구 경험치
	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Level")
    int32 RequiredExp = 0;  
    //총 경험치
    UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Level")
    int32 TotalExp = 0;  
};

//인터페이스 - 무기이펙트를 제공한다
class IWeaponTrailEffectProvider
{
public:
    virtual ~IWeaponTrailEffectProvider() = default;

    //파티클 시스템과 나이아가라 시스템 모두를 다룰 수 있다
    virtual UParticleSystem* GetLeftWeaponTrailEffectParticle() const = 0;
    virtual UNiagaraSystem* GetLeftWeaponTrailEffectNiagara() const = 0;

    virtual UParticleSystem* GetRightWeaponTrailEffectParticle() const = 0;
    virtual UNiagaraSystem* GetRightWeaponTrailEffectNiagara() const = 0;
};


//몬스터 데이터 테이블정보
USTRUCT(BlueprintType)
struct FMonsterTableInfo : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FString			Name;

    UPROPERTY(EditAnywhere)
    float			Attack;

    UPROPERTY(EditAnywhere)
    float			Defense;

    UPROPERTY(EditAnywhere)
    float			HP;

    UPROPERTY(EditAnywhere)
    float			MP;

    UPROPERTY(EditAnywhere)
    float			AttackDistance;

    UPROPERTY(EditAnywhere)
    float			DetectDistance;

    UPROPERTY(EditAnywhere)
    float			MoveSpeed;

    UPROPERTY(EditAnywhere)
    int32			Level;

    UPROPERTY(EditAnywhere)
    int32			Exp;

    UPROPERTY(EditAnywhere)
    int32			Gold;
};

//플레이어 데이터 테이블정보
USTRUCT(BlueprintType)
struct FPlayerTableInfo : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    EPlayerJob      Job;

    UPROPERTY(EditAnywhere)
    FString			Name;

    UPROPERTY(EditAnywhere)
    float			Attack;

    UPROPERTY(EditAnywhere)
    float			Defense;

    UPROPERTY(EditAnywhere)
    float			HP;

    UPROPERTY(EditAnywhere)
    float			MP;

    UPROPERTY(EditAnywhere)
    float			AttackDistance;

    UPROPERTY(EditAnywhere)
    float			MoveSpeed;

    UPROPERTY(EditAnywhere)
    int32			Level;

    UPROPERTY(EditAnywhere)
    int32			Exp;

    UPROPERTY(EditAnywhere)
    int32			Gold;
};


//애니메이션 템플릿에 사용되는 데이터테이블 구조체
USTRUCT(BlueprintType)
struct FPlayerAnimData : public FTableRowBase
{
    GENERATED_BODY()

public:
    //직업정보
    UPROPERTY(EditAnywhere)
    EPlayerJob          Job;

    //시퀀스플레이어
    UPROPERTY(EditAnywhere)
    TMap<FName, TObjectPtr<UAnimSequence>>  AnimMap;

    //이동블렌드스페이스
    UPROPERTY(EditAnywhere)
    TObjectPtr<UBlendSpace>     GroundBlendSpace;

    //에임오프셋
    UPROPERTY(EditAnywhere)
    TObjectPtr<UAimOffsetBlendSpace>    ViewAimOffSet;

    //공격몽타주
    UPROPERTY(EditAnywhere)
    TObjectPtr<UAnimMontage>        AttackMontage;

    //공격몽타주섹션이름
    UPROPERTY(EditAnywhere)
    TArray<FName>                           AttackSectionArray;

    //랜드몽타주
    UPROPERTY(EditAnywhere)
    TObjectPtr<UAnimMontage>        LandMontage;

};


//몬스터의 상태를 지정한다 -> 애니메이션과 행동, 엑터의 제거 등을 결정하는데 필요하다
UENUM(BlueprintType)
enum class EMonsterDefaultAnim : uint8
{
    Idle,
    Walk,
    Run,
    Attack,
    Death,
    Special
};

UENUM(BlueprintType)
enum class ESkillSoundType : uint8
{
BGM,
UISound,
RunSFX,
OnHitSFX,
WhenHitSFX
};

//케릭터의 오디오를 매핑하기 위한 구조체이다. TMap 으로 특정 키워드로 오디오에셋을 묶어준다. 
//아직 시스템에서 미사용중.. 언젠간 쓰자 ㅠ
USTRUCT(BlueprintType)
struct FCharacterAudioData : public FTableRowBase
{
    GENERATED_BODY()

public:
    //직업정보
    UPROPERTY(EditAnywhere)
    EPlayerJob          Job;

    //스킬SFX 데이터베이스 스킬이름과 
    // 애님데이터의 동작명과 일치시킬 것 이다 
    // 예 TEXT("Dash") == TEXT("Dash")
    UPROPERTY(EditAnywhere)
    TMap<FName, TObjectPtr<USoundBase>>  SkillSfxMap;
  
    //재생시 사운드를 루프해야 하는지 여부를 미리 저장해둔다 
    // 물론 사운드 재생하는 과정을 개별화 한다면 오버라이드 가능하다
    UPROPERTY(EditAnywhere)
    TMap<FName, bool> SkillLoopMap;

    //스킬 싱크를 위한 딜레이 맵이다 (타이밍을 맞추고자 할 때 읽어올 것임)
    UPROPERTY(EditAnywhere)
    TMap<FName, float>  SkillPlayDelayMap;

    //스킬 마다 재생할 위치를 지정할 수 있다
    UPROPERTY(EditAnywhere)
    TMap<FName, FVector> SkillLocationMap;

    //사운드 효과가 어떤 유형(주로 사용목적) 인지 구분하는 enum 이다
    UPROPERTY(EditAnywhere)
    TMap<FName, ESkillSoundType> SkillSoundTypeMap;
};

static TAutoConsoleVariable<int32> CVarDebugLines(
    TEXT("ShowDebugLines"),
    1,
    TEXT("디버그 라인 표시 여부 (1=켜기, 0=끄기)"),
    ECVF_Cheat
);

//몬스터 데이터 테이블정보
USTRUCT(BlueprintType)
struct FNPCTableInfo : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FString			Name;

    UPROPERTY(EditAnywhere)
    float			Attack;

    UPROPERTY(EditAnywhere)
    float			Defense;

    UPROPERTY(EditAnywhere)
    float			HP;

    UPROPERTY(EditAnywhere)
    float			MP;

    UPROPERTY(EditAnywhere)
    float			AttackDistance;

    UPROPERTY(EditAnywhere)
    float			DetectDistance;

    UPROPERTY(EditAnywhere)
    float			MoveSpeed;

    UPROPERTY(EditAnywhere)
    int32			Level;

    UPROPERTY(EditAnywhere)
    int32			Exp;

    UPROPERTY(EditAnywhere)
    int32			Gold;
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
    WEAPON,
    ARMOR,
    CONSUMABLE,
    TRADEGOODS
};

UENUM(BlueprintType)
enum class EItemWeaponType : uint8
{
    SWORDS,
    AXES,
    MACES,
    BOWS
};

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
    CRACKED,
    NORMAL,
    MAGIC,
    RARE,
    UNIQUE,
    ARTIFACT
};

template<typename T>
static FString GetEnumValueAsString(T number)
{
    const UEnum* EnumPtr = StaticEnum<T>();
    if (!EnumPtr)
        return TEXT("Invalid");

    return EnumPtr->GetNameStringByValue(static_cast<int64>(number));
}

//모든 아이템이 공통으로 사용하는 내용. 데이터테이블의 구조가된다.
USTRUCT(BlueprintType)
struct FItemMasterRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    FText								DisplayName;	        // UI 에 표시될 이름

    UPROPERTY(EditAnywhere)
    EItemType                           ItemType;           //아이템 유형

    UPROPERTY(EditAnywhere)
    TSoftObjectPtr<UDataTable>          DataTableRef;       //세부 데이터 테이블의 참조 -> 무기 방어구 컨슘 등

    UPROPERTY(EditAnywhere)
    FName                               RefRowName;          //세부 테이블의 행이름 RowName  (탐색용)

    UPROPERTY(EditAnywhere)
    TObjectPtr<UTexture2D>				Icon;		        //텍스처 타입의 아이템의 아이콘

    UPROPERTY(EditAnywhere)
    EItemRarity                         Rarity;             //아이템 희귀도

    UPROPERTY(EditAnywhere)
    int32                               ItemSellPrice;      //청산가격

    UPROPERTY(EditAnywhere)
    int32                               ItemBuyPrice;       //구매가격

    UPROPERTY(EditAnywhere)
    float                               ItemWeight;         //중량
};

//무기 아이템에서 추가로 사용하는 아이템 정보, 마스터 아이템 정보에 덧대면 된다
USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    EItemWeaponType     WeaponType;
    UPROPERTY(EditAnywhere)
    float               AttackPower;
    UPROPERTY(EditAnywhere)
    float               AttackSpeed;
    //데미지 속성
    //TSubclassOf<UDamageType> DamageType;
};

//방어구 아이템에서 추가로 사용하는 아이템 정보, 마스터 아이템 정보에 덧대면 된다
USTRUCT(BlueprintType)
struct FArmorData : public FTableRowBase
{
    GENERATED_BODY()

    // 방어구타입 -> 일단 방어구 슬롯 미구현
    // EEquipSlot     ArmorSlot;
    UPROPERTY(EditAnywhere)
    float               Defense;
    UPROPERTY(EditAnywhere)
    float               Durability;
  
};

//소비 아이템에서 추가로 사용하는 아이템 정보, 마스터 아이템 정보에 덧대면 된다
USTRUCT(BlueprintType)
struct FConsumableData : public FTableRowBase
{
    GENERATED_BODY()

    // 방어구타입 -> 일단 방어구 슬롯 미구현
    // EEquipSlot     ArmorSlot;
    UPROPERTY(EditAnywhere)
    float               Duration;
};

//플레이어의 스킬 데이터원형이다. 데이터테이블의 구조가된다
USTRUCT(BlueprintType)
struct FPlayerSkillData : public FTableRowBase
{
    GENERATED_BODY()
    UPROPERTY(EditAnywhere)
    EPlayerJob      Job;
    UPROPERTY(EditAnywhere)
    FString        Name;
    UPROPERTY(EditAnywhere)
    TObjectPtr<UTexture2D>  Icon;
    UPROPERTY(EditAnywhere)
    float           Damage;
    UPROPERTY(EditAnywhere)
    float           ManaCost;
};

//아이템의 희귀도별 컬러를 매핑하기 위해 사용 할 구조체이다
USTRUCT(BlueprintType)
struct FItemRarityColor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemRarity Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor Color;
};

//퀘스트 데이터 중 퀘스트의 계층레벨을 지정한다
UENUM(BlueprintType)
enum class EQuestLevel : uint8
{
    Section     UMETA(DisplayName = "Section"),  // 레벨 0
    MainQuest   UMETA(DisplayName = "Main Quest"),  // 레벨 1
    SubQuest    UMETA(DisplayName = "Sub Quest")   // 레벨 2
};

//퀘스트 데이터 중 작업의 타입을 지정한다
UENUM(BlueprintType)
enum class EQuestTaskType : uint8
{
    NONE      UMETA(DisplayName = "NONE"),
    ARRIVE      UMETA(DisplayName = "ARRIVE"),
    KILL       UMETA(DisplayName = "KILL"),
    LOOT        UMETA(DisplayName = "LOOT"),
    TALK        UMETA(DisplayName = "TALK")
};

//퀘스트 데이터 중 보상의 타입이다
UENUM(BlueprintType)
enum class EQuestRewardType : uint8
{
    NONE      UMETA(DisplayName = "NONE"),
    GOLD      UMETA(DisplayName = "GOLD"),
    ITEM       UMETA(DisplayName = "ITEM"),
    REPUTATION        UMETA(DisplayName = "REPUTATION")
};

//퀘스트의 데이터 원형이다. 이를 기반으로 작성한 데이터테이블을 CDO 를 통해 읽어서 사용하면 된다
USTRUCT(BlueprintType)
struct FQuestInfo : public FTableRowBase
{
    GENERATED_BODY()

public:
    // 퀘스트 고유 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName QuestID;

    // 계층 수준
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQuestLevel QuestLevel;

    // 상위 퀘스트 ID (레벨 2는 레벨 1을, 레벨 1은 레벨 0을 부모로 가짐)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ParentQuestID;

    //퀘스트 유형 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQuestTaskType  TaskType;

    // 퀘스트 제목
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Title;

    // 퀘스트 상세 설명
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = true))
    FString Description;

    // 퀘스트 목표 (예: '늑대 5마리 처치')
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ObjectiveText;

    // 정렬을 위한 순서 (같은 레벨 내에서)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SortOrder = 0;

    // 보상타입
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EQuestRewardType    RewardType;

    // 공통 보상 : exp 
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32               RewardExp;

    // 공통 보상 : Gold
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32               RewardGold;

    // 보상의 양 (골드 / 물약 / 평판포인트 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32               RewardAmount;

    // 아이템 코드 (보상이 아이템인 경우)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName               RewardItemCode;

    // 아이템 희귀도 (보상이 Rarity가 있는 아이템인 경우)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemRarity         RewardItemRarity;

    // 아이템 옵션코드 (보상이 옵션이 존재하는 아이템인 경우) -> 옵션 아직 미구현
    //UPROPERTY(EditAnywhere, BlueprintReadWrite)

    //TASK == ARRIVE 일 때 : 지정한 오버랩구역
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> TargetOverlapVolumeClass;	

    //TASK == KILL : 킬 행위에 지정할 대상
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<APawn>	TargetNPCClass;				

    //TASK == LOOT : 룻에 지정할 대상 (아이템,엑터)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor>	TargetPickupActorClass;		

    //TASK == 공통   (완료에 필요한 수량)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32				RequiredCount;				
};


//퀘스트 진행 정보를 저장할 떄 사용하는 열거형이다 
UENUM(BlueprintType)
enum class EQuestProgressState : uint8
{
    NOTACCEPTED,
    INPROGRESS,
    COMPLETED,
    REWARDCLAIMED
};