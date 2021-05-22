// Fill out your copyright notice in the Description page of Project Settings.


#include "NewConditionState.h"
#include "../NewMonster/NewMonsterController.h"

NewConditionState::NewConditionState(EMonsterConditionState type, float time, ANewMonsterController& control) :IsValid(true), Type(type), RemainTime(time), Controller(&control)
{
}

NewConditionState::~NewConditionState()
{
}
void NewConditionState::BeginState() {
	//처리 할거 있으면 처리
	Controller->ChangeConditionStateDel.Broadcast(Type);
}
void NewConditionState::TickState(float delta) {
	if (IsValid) {
		RemainTime -= delta;
		if (RemainTime <= 0) {
			EndState();
		}
	}
}
void NewConditionState::EndState() {
	Controller->ChangeConditionStateDel.Broadcast(EMonsterConditionState::E_EXIT);
	IsValid = false;
}