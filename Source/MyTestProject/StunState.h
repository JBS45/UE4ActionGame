// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StunState.generated.h"

class ABoss;

UCLASS()
class MYTESTPROJECT_API UStunState : public UObject
{
	GENERATED_BODY()
	
private:
	bool IsValid;
	float StunTime;
	float RemainTime;
	ABoss* Target;
private:
	void TickStun(float delta);
public:
	void BeginStun(ABoss* target,float time);
	void EndStun();
};
