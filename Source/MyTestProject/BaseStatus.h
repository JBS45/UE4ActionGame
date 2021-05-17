//Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyTestProject.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "BaseEnum.h"
#include "BaseStatus.generated.h"

const static FName PlayerTag(TEXT("Player"));
const static FName MonsterTag(TEXT("Monster"));


USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FBaseStatus
{
	GENERATED_USTRUCT_BODY()
public:

	float Hp;
	float Stamina;
	float Dmg;
	float Def;
	float CriticalRate;

public:
	const FBaseStatus operator+(const FBaseStatus& rhs);
	FBaseStatus& operator+= (const FBaseStatus& rhs);

};
USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FEquipmentStatus
{
	GENERATED_USTRUCT_BODY()
public:
	bool IsEmpty = true;
	uint8 ItemID;

	EEquipmentState EquipType;
	EWeaponType WeaponType;
	FBaseStatus StatusData;
};


USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FBaseCharacterStatus
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float MaxHp;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float CurrentHp;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float HpRegen;

	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float MaxStamina;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float CurrentStamina;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float StaminaRegen;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float StaminaConsumeSprint = 2.0f;

	float EvadeUseStaminaAmount = 20.0f;

	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float Dmg;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float Def;
	UPROPERTY(VisibleAnywhere, Category = "FinalStat")
		float CriticalRate;

	UPROPERTY(VisibleAnywhere, Category = "CharacterStat")
		FBaseStatus CharacterStatus;
	UPROPERTY(VisibleAnywhere, Category = "EquipSlot")
		TMap<EEquipmentState, FEquipmentStatus> Slot;

};

USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FHitBoxInfo
{
	GENERATED_USTRUCT_BODY()
public:

	FVector BoxSize;
	FVector BoxPos;

};

USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FBaseMonsterStatus
{
	GENERATED_USTRUCT_BODY()
public:

	float MaxHp;
	float Dmg;
	float Def;

	TMap<FString, float> AccumulateDamageForWeakpoint;
	TMap<FString, float> LimitDamageWeakpoint;
};
USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FAssistSkill : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CoolTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Animation;
public:
	bool IsReady = true;
	float CurrentLeftTime = 0.0f;
};
USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FMonsterAI{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMonsterState State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;
};
USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FMonsterAIData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ABaseMonster> MonsterType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBlackboardData* BlackBoard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FMonsterAI> MonsterAI;
};

USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FMonsterParts : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMonsterPartsType Part;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CanDestory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsWeak;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float DestroyDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Defence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName PartRootBoneName;
};

USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FMonsterStatus : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MonsterID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString MonsterName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName LeftHandSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName RightHandSocket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* PartsDataRef;
};
USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FMonsterAnim : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* Animation;
};


USTRUCT(BlueprintType)
struct FTotalCommandDataTable : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EWeaponType weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UDataTable* CommandTable;
};

USTRUCT(BlueprintType)
struct FMoveInputInfo {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		EMoveKey InputType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float  TimeStamp;
};
USTRUCT(BlueprintType)
struct FActionInputInfo {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		EActionKey InputType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
		float  TimeStamp;
};

USTRUCT(BlueprintType)
struct FChainAction {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECommandName AttackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString AttackName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ECommnadPriority Priority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EMoveKey> MoveCommand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<EActionKey> ActionCommand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* ActionMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ECommandName> EnableChainAction;
};
USTRUCT(BlueprintType)
struct FChainActionTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FChainAction Action;

};