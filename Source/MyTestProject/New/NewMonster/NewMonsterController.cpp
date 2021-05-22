// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMonsterController.h"
#include "MyNewBaseMonster.h"
#include "NewMonsterSpawner.h"
#include "Blueprint/AiBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../PlayerCharacter/MyNewCharacter.h"



const FName ANewMonsterController::TargetDistanceKey(TEXT("TargetDistance"));
const FName ANewMonsterController::TargetPlayerKey(TEXT("TargetPlayer"));
const FName ANewMonsterController::TargetLocationKey(TEXT("TargetLocation"));
const FName ANewMonsterController::MonsterState(TEXT("MonsterState"));
const FName ANewMonsterController::ConditionState(TEXT("ConditionState"));
const FName ANewMonsterController::RandomKey(TEXT("RandomChance"));


ANewMonsterController::ANewMonsterController() {
	CurrentMonsterState = ENewMonsterState::E_CREATE;
	CurrentConditionState = EMonsterConditionState::E_IDLE;
	BrokenState = 0;

	ChangeStateDel.AddUObject(this, &ANewMonsterController::ChangeMonsterState);
	ChangeConditionStateDel.AddUObject(this, &ANewMonsterController::ChangeMonsterConditionState);
}
void ANewMonsterController::OnPossess(APawn* InPawn) {
	Super::OnPossess(InPawn);
}

void ANewMonsterController::ChangeMonsterState(const ENewMonsterState state) {
	if (CurrentMonsterState == state) return;
	CurrentMonsterState = state;

	switch (CurrentMonsterState)
	{
		case ENewMonsterState::E_IDLE:
			SetBehaviorTree(ENewMonsterState::E_IDLE);
			ResetTarget();
			break;
		case ENewMonsterState::E_PLAYERFOUND:
			break;
		case ENewMonsterState::E_BATTLE:
			SetBehaviorTree(ENewMonsterState::E_BATTLE);
			break;
		case ENewMonsterState::E_DEAD:
			SetLifeSpan(10.0f);
			GetBrainComponent()->StopLogic(TEXT("Monster Is Dead"));
			ChangeStateDel.Clear();
			ChangeConditionStateDel.Clear();
			break;
	}
	SetBlackboardMonsterState(CurrentMonsterState);
}
void ANewMonsterController::ChangeMonsterConditionState(const EMonsterConditionState state) {
	if (CurrentConditionState == state) return;
	CurrentConditionState = state;
	switch (CurrentConditionState)
	{
	case EMonsterConditionState::E_IDLE:
		break;
	case EMonsterConditionState::E_ACTION:
		StopMovement();
		break;
	case EMonsterConditionState::E_KNOCKBACK:
		break;
	case EMonsterConditionState::E_STUN:
		break;
	case EMonsterConditionState::E_DOWN:
		break;
	}
	SetBlackboardConditionState(CurrentConditionState);
}
void ANewMonsterController::MonsterDead() {
	ChangeStateDel.Broadcast(ENewMonsterState::E_DEAD);
}

void ANewMonsterController::SetData(const TArray<FNewMonsterAI>& MonsterAI, UBlackboardData& BlackboardData, AMyNewBaseMonster& Monster, ANewMonsterSpawner& Area, const int32 ID) {
	CurrnetMonster = &Monster;
	MonsterID = ID;
	MonsterArea = &Area;
	CurrentBB = &BlackboardData;

	for (auto element : MonsterAI) {
		AITable.Add(element.State, element.BehaviorTree);
	}
}
void ANewMonsterController::SetTarget(AMyNewCharacter* player) {
	if (MonsterArea->GetIsPlayerInArea()&&Target==nullptr) {
		Target = player;
		Blackboard->SetValueAsObject(ANewMonsterController::TargetPlayerKey, Target);
		Blackboard->SetValueAsFloat(ANewMonsterController::TargetDistanceKey, MAX_FLT);
		ChangeStateDel.Broadcast(ENewMonsterState::E_PLAYERFOUND);
	}
}
void ANewMonsterController::ResetTarget() {
	if (Target != nullptr) {
		Target = nullptr;
		Blackboard->SetValueAsObject(ANewMonsterController::TargetPlayerKey, Target);
	}
}
void ANewMonsterController::SetBrokenState(ENewMonsterBrokenPart brokenPart) {
	BrokenState |= (int32)brokenPart;
}
void ANewMonsterController::SetBehaviorTree(const ENewMonsterState state) {
	UseBlackboard(CurrentBB, Blackboard);
	StopMovement();

	CurrentBT = AITable[state];
	RunBehaviorTree(CurrentBT);
}
void ANewMonsterController::SetBlackboardMonsterState(ENewMonsterState state)
{ 
	Blackboard->SetValueAsEnum(ANewMonsterController::MonsterState, (uint8)state);
}
void ANewMonsterController::SetBlackboardConditionState(EMonsterConditionState state) 
{
	Blackboard->SetValueAsEnum(ANewMonsterController::ConditionState, (uint8)state);
}