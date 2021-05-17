// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../MyTestProject.h"
#include "BehaviorTree/BTService.h"
#include "BTServiceCollection.generated.h"

UCLASS()
class MYTESTPROJECT_API UBTService_FindTargetForMonster : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_FindTargetForMonster();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

UCLASS()
class MYTESTPROJECT_API UBTService_SearchForMonster : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_SearchForMonster();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
UCLASS()
class MYTESTPROJECT_API UBTService_CheckCanChangeBattle : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckCanChangeBattle();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	float Timer;
};
UCLASS()
class MYTESTPROJECT_API UBTService_Random : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_Random();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

UCLASS()
class MYTESTPROJECT_API UBTService_CheckTargetInArea : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckTargetInArea();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

UCLASS()
class MYTESTPROJECT_API UBTService_CheckTargetDistance : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckTargetDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, Category = "MaxDistance", meta = (AllowPrivateAccess = "true"))
		float MaxChaseDistance;
};


UCLASS()
class MYTESTPROJECT_API UBTService_DecideForward : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_DecideForward();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

UCLASS()
class MYTESTPROJECT_API UBTService_CheckTargetAlive : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_CheckTargetAlive();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};