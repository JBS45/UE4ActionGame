// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NewMonsterStatus.h"
#include "NewBossrStatus.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTPROJECT_API UNewBossrStatus : public UNewMonsterStatus
{
	GENERATED_BODY()
	
public:
	UNewBossrStatus();
public:
	virtual int32 TakeDamage(const FHitResult& hit, float damage, float condDamage, bool& IsWeak) override;
private:
	int32 ConditionCount;
};
