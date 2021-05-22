// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NewMonsterBTTask.generated.h"

/**
 * 
 */
UCLASS()
class MYTESTPROJECT_API UBTTask_RandomPointInArea : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_RandomPointInArea();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, Category = "Move", meta = (AllowPrivateAccess = "true"))
		float AcceptableRadius;
};

UCLASS()
class MYTESTPROJECT_API UBTTask_ChangeToIDLE : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_ChangeToIDLE();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

UCLASS()
class MYTESTPROJECT_API UBTTask_ChangeToBattle : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_ChangeToBattle();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};

UCLASS()
class MYTESTPROJECT_API UBTTask_MonsterTurning : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_MonsterTurning();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	FRotator FindRotation(class AMyNewBaseMonster& monster);
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float LerpSpeed;
	FRotator Dest;
	FVector TargetVector;
};

UCLASS()
class MYTESTPROJECT_API UBTTask_PlayAnimMontage : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PlayAnimMontage();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	bool IsBegin = false;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Anim;
};

UCLASS()
class MYTESTPROJECT_API UBTTask_PlayAnimMontageForward : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PlayAnimMontageForward();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	bool IsBegin = false;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Anim;
	UPROPERTY(EditAnywhere, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float LerpSpeed;
	FVector ForwardDirection;
};

UCLASS()
class MYTESTPROJECT_API UBTTaskNode_ChasePlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_ChasePlayer();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, Category = "Node", meta = (AllowPrivateAccess = "true"))
		float AcceptableRadius = 30.0f;

};