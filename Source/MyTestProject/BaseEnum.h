// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MyTestProject.h"
#include "BaseEnum.generated.h"

UENUM(BlueprintType)
enum class EInputKey : uint8 {
	E_NONE UMETA(DisplayName = "NONE"),
	E_FORWARD UMETA(DisplayName = "FORWARD"),
	E_BACKWARD UMETA(DisplayName = "BACKWARD"),
	E_RIGHT UMETA(DisplayName = "RIGHT"),
	E_LEFT UMETA(DisplayName = "LEFT"),
	E_EVADE UMETA(DisplayName = "EVADE"),
	E_LEFTCLICK UMETA(DisplayName = "LEFTCLICK"),
	E_RIGHTCLICK UMETA(DisplayName = "RIGHTCLICK"),
	E_LEFTANDRIGHTCLICK UMETA(DisplayName = "LEFTANDRIGHTCLICK"),
};
UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	E_NONE UMETA(DisplayName = "None"),
	E_IDLE UMETA(DisplayName = "Idle"),
	E_BATTLE UMETA(DisplayName = "Battle"),
	E_HIT UMETA(DisplayName = "Hit"),
	E_DOWN UMETA(DisplayName = "Down"),
	E_DEAD UMETA(DisplayName = "Dead"),
};
UENUM(BlueprintType)
enum class EEquipmentState : uint8 {
	E_WEAPON = 0 UMETA(DisplayName = "Weapon"),
	E_HEAD UMETA(DisplayName = "Head"),
	E_CHEST UMETA(DisplayName = "Chest"),
	E_LEG UMETA(DisplayName = "Leg"),
};
UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	E_NOWEAPON = 0 UMETA(DisplayName = "NOWEAPON"),
	E_DUAL = 1 UMETA(DisplayName = "DUAL"),
	E_HAMMER = 3 UMETA(DisplayName = "Hammer"),
};

UENUM(BlueprintType)
enum class ECommandName : uint8 {
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

};
UENUM(BlueprintType)
enum class EDetectCollisionType : uint8 {
	E_NONE = 0 UMETA(DisplayName = "none"),
	E_PLAYER = 1 UMETA(DisplayName = "player"),
	E_MONSTER = 2 UMETA(DisplayName = "monster"),
};
UENUM(BlueprintType)
enum class EMonsterState : uint8 {
	E_CREATE = 0 UMETA(DisplayName = "NONE"),
	E_IDLE = 1 UMETA(DisplayName = "IDLE"),
	E_BATTLE = 2 UMETA(DisplayName = "BATTLE"),
	E_RAGE = 3 UMETA(DisplayName = "RAGE"),
	E_DEAD = 4 UMETA(DisplayName = "DEAD"),
};

UENUM(BlueprintType)
enum class EMonsterPartsType : uint8 {
	E_HEAD = 0 UMETA(DisplayName = "Head"),
	E_BODY = 1 UMETA(DisplayName = "Body"),
	E_LEFTHAND = 2 UMETA(DisplayName = "LeftHand"),
	E_RIGHTHAND = 3 UMETA(DisplayName = "RightHand"),
	E_WING = 4 UMETA(DisplayName = "Wing"),
	E_LEFTLEG = 5 UMETA(DisplayName = "LeftLeg"),
	E_RIGHTLEG = 6 UMETA(DisplayName = "RightLeg"),
	E_TAIL = 7 UMETA(DisplayName = "Tail"),
	E_ALLBODY = 8 UMETA(DisplayName = "ALL")
};

UENUM(BlueprintType)
enum class EDamageType : uint8 {
	E_NORMAL = 0 UMETA(DisplayName = "Normal"),
	E_KNOCKBACK = 1 UMETA(DisplayName = "KnockBack"),
	E_KNOCKDOWN = 2 UMETA(DisplayName = "KnockDown"),
	E_STUN = 3 UMETA(DisplayName = "Stun"),
	E_ROAR = 4 UMETA(DisplayName = "Roar"),
};
UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EMonsterBrokenParts {
	E_NONE = 0 UMETA(Hidden),
	E_HEAD = 1 UMETA(DisplayName = "HEAD"),
	E_BODY = 1 << 1 UMETA(DisplayName = "BODY"),
	E_LEFTHAND = 1 << 2 UMETA(DisplayName = "LEFTHAND"),
	E_RIGHTHAND = 1 << 3 UMETA(DisplayName = "RIGHTHAND"),
	E_WING = 1 << 4 UMETA(DisplayName = "WING"),
	E_LEFTLEG = 1 << 5 UMETA(DisplayName = "LEFTLEG"),
	E_RIGHTLEG = 1 << 6 UMETA(DisplayName = "RIGHTLEG"),
	E_TAIL = 1 << 7 UMETA(DisplayName = "TAIL"),
};
ENUM_CLASS_FLAGS(EMonsterBrokenParts);

UENUM(Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ECharacterStaminaUse {
	E_IDLE = 0 UMETA(Hidden),
	E_SPRINT = 1 UMETA(DisplayName = "Sprint"),
	E_SPECIAL = 1 << 1 UMETA(DisplayName = "SPECIAL"),
};
ENUM_CLASS_FLAGS(ECharacterStaminaUse);

UENUM(Meta = (Bitflags))
enum class ECommnadPriority {
	E_LOW = 0 UMETA(DisplayName = "Low"),
	E_NORMAL = 1 UMETA(DisplayName = "Normal"),
	E_HIGH = 2 UMETA(DisplayName = "High"),
};

UENUM(BlueprintType)
enum class EMoveKey : uint8 {
	E_FORWARD = 0 UMETA(DisplayName = "W"),
	E_BACKWARD = 1 UMETA(DisplayName = "S"),
	E_RIGHT = 2 UMETA(DisplayName = "D"),
	E_LEFT = 3 UMETA(DisplayName = "A"),
	E_ALLMOVE = 4 UMETA(DisplayName = "AnyMoveKey"),

};

UENUM(BlueprintType)
enum class EActionKey : uint8 {

	E_EVADE = 0 UMETA(DisplayName = "SPACEBAR"),
	E_LEFTCLICK = 1  UMETA(DisplayName = "LMB"),
	E_RIGHTCLICK = 2 UMETA(DisplayName = "RMB"),
	E_THUMBCLICK = 3 UMETA(DisplayName = "THUMBMB"),
	E_SPECIAL = 4 UMETA(DisplayName = "Special"),
};



DECLARE_DELEGATE(FVoidDelegate);
DECLARE_DELEGATE_OneParam(FVoidDelegateOne, ECharacterState);
DECLARE_DELEGATE_OneParam(FVoidDelegateBool, bool);
DECLARE_DELEGATE_OneParam(FVoidDelegateAnimMontage, UAnimMontage*);
DECLARE_DELEGATE_FourParams(FPlayerAttackDel, class ABasePlayerController*, FVector, int32, bool);
DECLARE_DELEGATE_RetVal(bool, FCheckCanEvadStamina);


DECLARE_MULTICAST_DELEGATE(FVoidMultiDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterState, ECharacterState);
DECLARE_MULTICAST_DELEGATE_OneParam(FWeaponType, EWeaponType);
DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterSprint, bool);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);