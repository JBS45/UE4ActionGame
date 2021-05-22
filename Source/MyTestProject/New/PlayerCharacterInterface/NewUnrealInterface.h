// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyNewInputInterface.h"
#include "../NewMonsterComponents/NewMonsterStruct.h"
#include "NewUnrealInterface.generated.h"
/**
 * 
 */
UINTERFACE(Blueprintable)

class MYTESTPROJECT_API UResetInputBuffer: public UInterface
{
	GENERATED_BODY()

};
class MYTESTPROJECT_API IResetInputBuffer
{
	GENERATED_BODY()
public:
	virtual void NotifyReset() PURE_VIRTUAL(UResetInputBuffer::NotifyReset);
};


UINTERFACE(Blueprintable)

class MYTESTPROJECT_API UChangeCommand : public UInterface
{
	GENERATED_BODY()

};
class MYTESTPROJECT_API IChangeCommand
{
	GENERATED_BODY()
public:
	virtual void NotifyCommand(TArray<FNewChainAction> EnableAction) PURE_VIRTUAL(UChangeCommand::NotifyCommand);
};

UINTERFACE(Blueprintable)

class MYTESTPROJECT_API UUpdateStatus : public UInterface
{
	GENERATED_BODY()

};
class MYTESTPROJECT_API IUpdateStatus
{
	GENERATED_BODY()
public:
	virtual void NotifyStatus(float maxhp,float currhp,float maxstatmina,float currstamina) PURE_VIRTUAL(UChangeCommand::NotifyCommand);
};

UINTERFACE(Blueprintable)

class MYTESTPROJECT_API UNewDamageInterface : public UInterface
{
	GENERATED_BODY()

};
class MYTESTPROJECT_API INewDamageInterface
{
	GENERATED_BODY()
public:
	virtual void ApplyAttack(const FHitResult& Hit,float damage,float condDamage,float critical) PURE_VIRTUAL(UNewDamageInterface::ApplyAttack);
	virtual int32 TakeAttack(const FHitResult& Hit, float damage, float condDamage, class ACharacter* damageCauser, bool& IsWeak, ENewMonsterDamageType type, float knockback) PURE_VIRTUAL(UNewDamageInterface::TakeAttack, return 0;);
	virtual bool IsAlive() PURE_VIRTUAL(UNewDamageInterface::IsAlive, return true;);
};
