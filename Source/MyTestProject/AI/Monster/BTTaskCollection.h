// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../MyTestProject.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskCollection.generated.h"



UCLASS()
class MYTESTPROJECT_API UBTTask_MonsterPatrol : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_MonsterPatrol();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

UCLASS()
class MYTESTPROJECT_API UBTTask_FindClosePatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_FindClosePatrolPoint();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
UCLASS()
class MYTESTPROJECT_API UBTTask_PlayAnim : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PlayAnim();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	bool IsBegin = false;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Anim;
};

UCLASS()
class MYTESTPROJECT_API UBTTask_PlayAnimToForward : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PlayAnimToForward();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	bool IsBegin = false;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Anim;
	UPROPERTY(EditAnywhere, Category = "Distance", meta = (AllowPrivateAccess = "true"))
		float distance;
};

UCLASS()
class MYTESTPROJECT_API UBTTask_PlayTwoAnim : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_PlayTwoAnim();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	bool IsBegin = false;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Anim;
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		float Degree;
	float Lerpspeed;
	FRotator DestRotation;
};

UCLASS()
class MYTESTPROJECT_API UBTTask_TurnForMonster : public UBTTaskNode
{
	GENERATED_BODY()
public:

	UBTTask_TurnForMonster();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, Category = "Rotate", meta = (AllowPrivateAccess = "true"))
		float LerpSpped;
	FRotator Dest;
	FVector TargetVector;
private:
	FRotator FindRotation(class ABaseMonster* ControllingPawn);
};

UCLASS()
class MYTESTPROJECT_API UBTTaskNode_ChaseTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_ChaseTarget();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, Category = "Node", meta = (AllowPrivateAccess = "true"))
		float AcceptableRadius = 30.0f;

	class ABaseCharacter* Target;
	bool bBegin = false;
};

UCLASS()
class MYTESTPROJECT_API UBTTaskNode_ChaseFail : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_ChaseFail();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	UPROPERTY(EditAnywhere, Category = "Node", meta = (AllowPrivateAccess = "true"))
		float AcceptableRadius = 30.0f;

	class ABaseCharacter* Target;
	bool bBegin = false;
};

UCLASS()
class MYTESTPROJECT_API UBTTask_Roar : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Roar();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* Anim;
};