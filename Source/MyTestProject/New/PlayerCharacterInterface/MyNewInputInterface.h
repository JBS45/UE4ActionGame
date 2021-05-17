// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MyNewPlayerInterface.h"
#include "MyNewInputInterface.generated.h"


 UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EMoveKeyFlag {
	E_NONE = 0 UMETA(DisplayName = "None"),
	E_FORWARD = 1 UMETA(DisplayName = "W"),
	E_BACKWARD = 1<<1 UMETA(DisplayName = "S"),
	E_RIGHT = 1 << 2 UMETA(DisplayName = "D"),
	E_LEFT = 1 << 3 UMETA(DisplayName = "A"),
};
ENUM_CLASS_FLAGS(EMoveKeyFlag);

UENUM(BlueprintType)
enum class EInputBufferState : uint8 {
	E_ENABLE = 0 UMETA(DisplayName = "Enable"),
	E_ONLYMOVE= 1 UMETA(DisplayName = "OnlyMove"),
	E_ONLYACTION = 2 UMETA(DisplayName = "OnlyAction"),
	E_DISABLE = 3 UMETA(DisplayName = "Disable"),
};

UENUM(BlueprintType)
enum class ENewActionKey : uint8 {

	E_NONE = 0  UMETA(DisplayName = "None"),
	E_SPACEBAR = 1 UMETA(DisplayName = "SPACEBAR"),
	E_LEFTCLICK = 2  UMETA(DisplayName = "LMB"),
	E_RIGHTCLICK = 3 UMETA(DisplayName = "RMB"),
	E_LEFTRIGHTCLICK = 4 UMETA(DisplayName = "LRMB"),
	E_THUMBCLICK = 5 UMETA(DisplayName = "THUMBMB"),
	E_E = 6 UMETA(DisplayName = "E"),
	E_C = 7 UMETA(DisplayName = "C"),
	E_TAB = 8 UMETA(DisplayName = "TAB"),
	E_SHIFT = 9 UMETA(DisplayName = "SHIFT"),
};

UENUM(BlueprintType)
enum class ENewCommandName : uint8 {
	E_BASE = 0 UMETA(DisplayName = "Base"),
	E_DRAW = 1 UMETA(DisplayName = "Draw"),
	E_PUTUP = 2 UMETA(DisplayName = "PutUp"),
	E_SPECIAL = 3 UMETA(DisplayName = "Special"),
	E_SPECIALDRAW = 4 UMETA(DisplayName = "SpecialDraw"),
	E_LATK1 = 5 UMETA(DisplayName = "LeftAttack1"),
	E_LATK2 = 6 UMETA(DisplayName = "LeftAttack2"),
	E_LATK3 = 7 UMETA(DisplayName = "LeftAttack3"),
	E_LATK4 = 8 UMETA(DisplayName = "LeftAttack4"),
	E_LATK5 = 9 UMETA(DisplayName = "LeftAttack5"),
	E_LATK6 = 10 UMETA(DisplayName = "LeftAttack6"),
	E_LATK7 = 11 UMETA(DisplayName = "LeftAttack7"),
	E_LATK8 = 12 UMETA(DisplayName = "LeftAttack8"),
	E_LATK9 = 13 UMETA(DisplayName = "LeftAttack9"),
	E_LATK10 = 14 UMETA(DisplayName = "LeftAttack10"),
	E_RATK1 = 15 UMETA(DisplayName = "RighttAttack1"),
	E_RATK2 = 16 UMETA(DisplayName = "RighttAttack2"),
	E_RATK3 = 17 UMETA(DisplayName = "RighttAttack3"),
	E_RATK4 = 18 UMETA(DisplayName = "RighttAttack4"),
	E_RATK5 = 19 UMETA(DisplayName = "RighttAttack5"),
	E_RATK6 = 20 UMETA(DisplayName = "RighttAttack6"),
	E_RATK7 = 21 UMETA(DisplayName = "RighttAttack7"),
	E_RATK8 = 22 UMETA(DisplayName = "RightAttack8"),
	E_RATK9 = 23 UMETA(DisplayName = "RightAttack9"),
	E_RATK10 = 24 UMETA(DisplayName = "RightAttack10"),
	E_LRATK1 = 25 UMETA(DisplayName = "LRAttack1"),
	E_LRATK2 = 26 UMETA(DisplayName = "LRAttack2"),
	E_LRATK3 = 27 UMETA(DisplayName = "LRAttack3"),
	E_EVADE = 28 UMETA(DisplayName = "Evade"),
	E_LEFTSTEP = 29 UMETA(DisplayName = "LeftStep"),
	E_RIGHTSTEP = 30 UMETA(DisplayName = "RightStep"),
	E_CAST = 31 UMETA(DisplayName = "Cast"),
	E_PUTUPCAST = 32 UMETA(DisplayName = "PutUpCast"),
	E_POTION = 33 UMETA(DisplayName = "Potion"),
	E_PUTUPPOTION = 34 UMETA(DisplayName = "PutUpPotion"),
	E_EXCHANGE = 35 UMETA(DisplayName = "Exchagne"),
	E_EXCHANGEATTACK = 36 UMETA(DisplayName = "ExchangeAttack"),

};

USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FMoveKeyState
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "Key", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "EMoveKeyFlag"))
		int32 KeyFlag;
	UPROPERTY(VisibleAnywhere, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeStamp;
};

USTRUCT(BlueprintType)
struct MYTESTPROJECT_API FActionKeyState
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(VisibleAnywhere, Category = "Key", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "EMoveKeyFlag"))
		ENewActionKey Key;
	UPROPERTY(VisibleAnywhere, Category = "Time", meta = (AllowPrivateAccess = "true"))
		float TimeStamp;
};

USTRUCT(BlueprintType)
struct FNewChainAction {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString AttackName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ENewCommandName AttackType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsGUIVisible;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 Priority;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "EMoveKeyFlag"))
		int32 MoveCommand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ENewActionKey ActionCommand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UAnimMontage* ActionMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ENewCommandName> EnableChainAction;
};
USTRUCT(BlueprintType)
struct FNewChainActionTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNewChainAction Action;

};
USTRUCT(BlueprintType)
struct FNewTotalCommandTable : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ENewWeaponType weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UDataTable* CommandTable;
};