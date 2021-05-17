// Fill out your copyright notice in the Description page of Project Settings.


#include "StunState.h"
#include "Monster/Boss.h"

void UStunState::BeginStun(ABoss* target,float time) {

	IsValid = true;
	Target = target;
	StunTime = time;
	RemainTime = time;
	Target->SetIsStun(true);
	Target->GetAnimInst()->PlayStun();
}
void UStunState::TickStun(float delta) {
	if (IsValid) {
		RemainTime -= delta;
		if (RemainTime <= 0) {
			EndStun();
		}
	}

}
void UStunState::EndStun() {
	IsValid = false;
	Target->SetIsStun(false);
}