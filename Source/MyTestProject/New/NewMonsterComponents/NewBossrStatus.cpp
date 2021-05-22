// Fill out your copyright notice in the Description page of Project Settings.


#include "NewBossrStatus.h"
#include "../NewMonster/MyNewBaseMonster.h"


UNewBossrStatus::UNewBossrStatus() {
	ConditionCount = 0;
}

int32 UNewBossrStatus::TakeDamage(const FHitResult& hit, float damage, float condDamage, bool& IsWeak)
{
	if (Cast<INewDamageInterface>(Owner)->IsAlive() == false) return -1;

	MonsterPartsDataInGame* TargetPart = HitBox[hit.BoneName].Get();
	float Defence = TargetPart->Defence;
	float DefenceRate = Defence / 100.0f;
	const int32 FinalDamage = FMath::FloorToInt(damage * (1 - DefenceRate));
	const int32 FinalCondDamage = FMath::FloorToInt(condDamage * (1 - DefenceRate));

	TargetPart->AccruedDamage += FinalDamage;
	TargetPart->AccruedConditionDamage += FinalCondDamage;

	IsWeak = TargetPart->IsWeak;

	CurrentHp -= FinalDamage;
	if (CurrentHp <= 0) {
		MonsterDeadDel.ExecuteIfBound();
	}

	if (CheckBreakPart(*TargetPart)) {
		Owner->SetBrokenPart(TargetPart->Type);
	}
	if (CheckConditionPart(*TargetPart)) {
		if (TargetPart->ConditionType == EMonsterConditionType::E_IDLE) {
			Owner->SetCondition(EMonsterConditionState::E_STUN, 5.0f + (ConditionCount*3.0f));
		}
		else {
			Owner->SetCondition(EMonsterConditionState::E_DOWN, 9.0f + (ConditionCount*4.0f));
		}
	}

	return FinalDamage;
}
