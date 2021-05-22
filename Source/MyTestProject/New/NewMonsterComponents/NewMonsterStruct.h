// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NewMonsterStruct.generated.h"

UENUM(BlueprintType)
enum class ENewMonsterPartsType : uint8 {
	E_HEAD = 0 UMETA(DisplayName = "Head"),
	E_BODY = 1 UMETA(DisplayName = "Body"),
	E_LEFTHAND = 2 UMETA(DisplayName = "LeftHand"),
	E_RIGHTHAND = 3 UMETA(DisplayName = "RightHand"),
	E_LEFTLEG = 4 UMETA(DisplayName = "LeftLeg"),
	E_RIGHTLEG = 5 UMETA(DisplayName = "RightLeg"),
	E_TAIL = 6 UMETA(DisplayName = "Tail"),
};

UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ENewMonsterBrokenPart {
	E_NONE = 0 UMETA(Hidden),
	E_HEAD = 1 UMETA("HEAD"),
	E_BODY = 1<<1 UMETA(DisplayName = "BODY"),
	E_LEFTHAND = 1 << 2 UMETA(DisplayName = "LEFTHAND"),
	E_RIGHTHAND = 1 << 3 UMETA(DisplayName = "RIGHTHAND"),
	E_LEFTLEG = 1 << 4 UMETA(DisplayName = "LEFTLEG"),
	E_RIGHTLEG = 1 << 5 UMETA(DisplayName = "RIGHTLEG"),
	E_TAIL = 1 << 6 UMETA(DisplayName = "Tail"),

};
ENUM_CLASS_FLAGS(ENewMonsterBrokenPart);

UENUM(BlueprintType)
enum class EMonsterConditionType : uint8 {
	E_IDLE = 0 UMETA(DisplayName = "STUN"),
	E_DOWN = 1 UMETA(DisplayName = "DOWN"),

};
UENUM(BlueprintType)
enum class EMonsterConditionState : uint8 {
	E_IDLE = 0 UMETA(DisplayName = "IDLE"),
	E_ACTION = 1 UMETA(DisplayName = "ACTION"),
	E_KNOCKBACK = 2 UMETA(DisplayName = "KNOCKBACK"),
	E_STUN = 3 UMETA(DisplayName = "STUN"),
	E_DOWN = 4 UMETA(DisplayName = "DOWN"),
	E_EXIT = 5 UMETA(DisplayName = "EXIT"),

};

UENUM(BlueprintType)
enum class ENewMonsterState : uint8 {
	E_CREATE = 0 UMETA(DisplayName = "Create"),
	E_IDLE = 1 UMETA(DisplayName = "Idle"),
	E_PLAYERFOUND = 2 UMETA(DisplayName = "PlayerFound"),
	E_BATTLE = 3 UMETA(DisplayName = "Battle"),
	E_DEAD = 4 UMETA(DisplayName = "Dead"),
};
UENUM(BlueprintType)
enum class ENewMonsterAttackPart : uint8 {
	E_LEFT = 0 UMETA(DisplayName = "LEFT"),
	E_RIGHT = 1 UMETA(DisplayName = "RIGHT"),
	E_LEFTRIGHT = 2 UMETA(DisplayName = "LEFTRIGHT"),
};

UENUM(BlueprintType)
enum class ENewMonsterDamageType : uint8 {
	E_NORMAL = 0 UMETA(DisplayName = "Normal"),
	E_KNOCKBACK = 1 UMETA(DisplayName = "KnockBack"),
	E_ROAR = 2 UMETA(DisplayName = "Roar"),
};


USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FNewMonsterAI {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ENewMonsterState State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;
};
USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FMonsterPartsData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ENewMonsterPartsType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EMonsterConditionType ConditionType;
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
struct MYTESTPROJECT_API FNewMonsterData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		uint8 MonsterID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USkeletalMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Size = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Hp = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float Damage = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* PartsData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AMyNewBaseMonster> MonsterActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class UAnimInstance> MonsterAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBlackboardData* BlackBoard;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FNewMonsterAI> MonsterAI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class ANewMonsterProjectile> Projectile;

};

