// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MyNewPlayerInterface.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ENewWeaponType : uint8 {
	E_NONE =0 UMETA(Hidden),
	E_DUAL = 1 UMETA(DisplayName = "DUAL"),
	E_KATANA = 2 UMETA(DisplayName = "KATANA"),
	E_AXE = 3 UMETA(DisplayName = "AXE"),
};
UENUM(BlueprintType)
enum class ENewPlayerState : uint8 {
	E_IDLE = 0 UMETA(DisplayName = "Idle"),
	E_SPRINT = 1 UMETA(DisplayName = "Sprint"),
	E_BATTLE= 2 UMETA(DisplayName = "Battle"),
	E_DEAD = 3 UMETA(DisplayName = "Dead"),
};
UENUM(BlueprintType)
enum class ENewActionState : uint8 {
	E_NONE = 0 UMETA(DisplayName = "None"),
	E_ACTION = 1 UMETA(DisplayName = "Action"),
	E_AFTERACTION = 2 UMETA(DisplayName = "AfterAction"),
	E_HIT = 3 UMETA(DisplayName = "Hit"),
	E_KNOCKBACK = 4 UMETA(DisplayName = "KnockBack"),
	E_DOWN = 5 UMETA(DisplayName = "Down"),
	E_EVADE = 6 UMETA(DisplayName = "Evade"),
	E_GETUP = 7 UMETA(DisplayName = "GetUp"),
};

UENUM(BlueprintType)
enum class EWeaponHand : uint8 {
	E_RIGHT = 0 UMETA(DisplayName = "RIght"),
	E_LEFT = 1 UMETA(DisplayName = "Left"),

};

UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ENewStaminaState {
	E_IDLE = 0 UMETA(Hidden),
	E_SPRINT = 1 UMETA(DisplayName = "Sprint"),
	E_SPECIAL = 1 << 1 UMETA(DisplayName = "DualSpecial"),
	E_DEAD = 1<<2 UMETA(DisplayName = "Dead"),
};
ENUM_CLASS_FLAGS(ENewStaminaState);

USTRUCT(BlueprintType)
struct FNewWeaponData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ENewWeaponType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float BaseDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float ConditionDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CriticalRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TraceSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMesh* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class AMyNewBaseWeapon> WeaponActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USoundCue* SwingSound;

};

USTRUCT(BlueprintType)
struct FNewPlayerStatus : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaxStamina;


};