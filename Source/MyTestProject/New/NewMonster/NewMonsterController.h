// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyNewBaseMonster.h"
#include "../NewMonsterComponents/NewMonsterStruct.h"
#include "NavigationSystem.h"
#include "NewMonsterController.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FChangeMonsterState, ENewMonsterState);
DECLARE_MULTICAST_DELEGATE_OneParam(FChangeMonsterConditionState, EMonsterConditionState);


class AMyNewBaseMonster;
class UBehaviorTree;
class UBlackboardData;
class AMyNewCharacter;
class ANewMonsterSpawner;

UCLASS()
class MYTESTPROJECT_API ANewMonsterController : public AAIController
{
	GENERATED_BODY()
	
public:
	ANewMonsterController();

	virtual void OnPossess(APawn* InPawn) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		AMyNewBaseMonster* CurrnetMonster;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster", meta = (AllowPrivateAccess = "true"))
		int32 MonsterID;


	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		ENewMonsterState CurrentMonsterState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true"))
		EMonsterConditionState CurrentConditionState;
	UPROPERTY(VisibleAnywhere, Category = "State", meta = (AllowPrivateAccess = "true", Bitmask, BitmaskEnum = "EMonsterBrokenParts"))
		int32 BrokenState;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		TMap<ENewMonsterState, UBehaviorTree*> AITable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UBehaviorTree* CurrentBT;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UBlackboardData* CurrentBB;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Area", meta = (AllowPrivateAccess = "true"))
		ANewMonsterSpawner* MonsterArea;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Target", meta = (AllowPrivateAccess = "true"))
		AMyNewCharacter* Target;

public:
	static const FName TargetDistanceKey;
	static const FName TargetPlayerKey;
	static const FName TargetLocationKey;
	static const FName MonsterState;
	static const FName ConditionState;
	static const FName RandomKey;

public:
	FChangeMonsterState ChangeStateDel;
	FChangeMonsterConditionState ChangeConditionStateDel;

public:
	void SetData(const TArray<FNewMonsterAI>& MonsterAI, UBlackboardData& BlackboardData,AMyNewBaseMonster& Monster, ANewMonsterSpawner& Area, const int32 ID);
	void SetTarget(AMyNewCharacter* player);
	void ResetTarget();
	void SetBrokenState(ENewMonsterBrokenPart brokenPart);


	void ChangeMonsterState(const ENewMonsterState state);
	void ChangeMonsterConditionState(const EMonsterConditionState state);
	void MonsterDead();
public:
	FORCEINLINE AMyNewCharacter* GetTarget() const { return Target; }
	FORCEINLINE AMyNewBaseMonster* GetCurrnetMonster() const { return CurrnetMonster; }
	FORCEINLINE EMonsterConditionState GetCurrentConditionState() const { return CurrentConditionState; }
private:
	void SetBlackboardMonsterState(ENewMonsterState state);
	void SetBlackboardConditionState(EMonsterConditionState state);
	void SetBehaviorTree(const ENewMonsterState state);
};
