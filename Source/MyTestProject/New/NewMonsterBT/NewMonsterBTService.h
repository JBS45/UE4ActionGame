// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "NewMonsterBTService.generated.h"

/**
 * 
 */

UCLASS()
class MYTESTPROJECT_API UBTService_CheckDistance : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

UCLASS()
class MYTESTPROJECT_API UBTService_MakeRandom : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_MakeRandom();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

UCLASS()
class MYTESTPROJECT_API UBTService_CheckTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};