// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../NewMonsterComponents/NewMonsterStruct.h"

class ANewMonsterController;

class MYTESTPROJECT_API NewConditionState
{
public:
	NewConditionState(EMonsterConditionState type,float time, ANewMonsterController& control);
	~NewConditionState();
private:
	bool IsValid;
	EMonsterConditionState Type;
	float RemainTime;
	ANewMonsterController* Controller;
public:
	void BeginState();
	void TickState(float delta);
	void EndState();
	FORCEINLINE bool GetIsValid() { return IsValid; };
};
