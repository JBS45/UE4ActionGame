// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../MyTestProject.h"
#include "../BaseEnum.h"
#include "../BaseStatus.h"
#include "MyInterface.generated.h"
/**
 *
 */

UINTERFACE(Blueprintable)

class MYTESTPROJECT_API UDamageInterface : public UInterface
{
	GENERATED_BODY()

};
class MYTESTPROJECT_API IDamageInterface
{
	GENERATED_BODY()
public:
	virtual void ApplyDamageFunc(const FHitResult& hit, const float AcitonDamageRate, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0) PURE_VIRTUAL(UDamageInterface::ApplyDamageFunc);
	virtual void TakeDamageFunc(bool& OutIsWeak, int32& OutFinalDamage, AActor* Causer, const FHitResult& hit, const float CaculateDamage, const EDamageType DamageType = EDamageType::E_NORMAL, const float ImpactForce = 0)  PURE_VIRTUAL(UDamageInterface::TakeDamageFunc);
};

UINTERFACE(Blueprintable)

class MYTESTPROJECT_API UStatusInterface : public UInterface
{
	GENERATED_BODY()

};
class MYTESTPROJECT_API IStatusInterface
{
	GENERATED_BODY()
public:
	virtual void NotifyStatusData(const FBaseCharacterStatus CharacterStatus) PURE_VIRTUAL(IStatusInterface::NotifyStatusData);
};


UINTERFACE(Blueprintable)

class MYTESTPROJECT_API UChainInterface : public UInterface
{
	GENERATED_BODY()

};
class MYTESTPROJECT_API IChainInterface
{
	GENERATED_BODY()
public:
	virtual void NotifyChainData(const TArray<FChainAction> chain) PURE_VIRTUAL(UChainInterface::NotifyChainData);
};

UINTERFACE(Blueprintable)

class MYTESTPROJECT_API UCameraLockOnInterface : public UInterface
{
	GENERATED_BODY()

};
class MYTESTPROJECT_API ICameraLockOnInterface
{
	GENERATED_BODY()
public:
	virtual void NotifyLockOnData(bool IsLockOn, class ABaseMonster* Target) PURE_VIRTUAL(IChainInterface::NotifyLockOnData);
};
