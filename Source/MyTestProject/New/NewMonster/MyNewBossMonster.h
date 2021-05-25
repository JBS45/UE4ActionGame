// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyNewBaseMonster.h"
#include "MyNewBossMonster.generated.h"

class NewConditionState;
class ANewMonsterProjectile;

UCLASS()
class MYTESTPROJECT_API AMyNewBossMonster : public AMyNewBaseMonster
{
	GENERATED_BODY()
	
public:
	AMyNewBossMonster();
	virtual void Tick(float DeltaTime) override;

public:
	virtual void SetCondition(EMonsterConditionState type, float totalTime) override;
	virtual void SetBrokenPart(ENewMonsterPartsType part) override;

	virtual void SetUpMonster(const FNewMonsterData& data, ANewMonsterSpawner* area);
private:
	void CheckConditionState(float delta);
private:
	TArray<TSharedPtr<NewConditionState>> ConditionStateArray;

	UMaterial* BaseMat;
	UMaterialInstanceDynamic* ArmMatInst;
public:
	void RadialAttack(float Range);
	ANewMonsterProjectile* SpawnProjectile(float Range,float damageRate,float Knockback);
	void ShootProjectile(ANewMonsterProjectile* rock);
private:
	void BreakArm();
};
