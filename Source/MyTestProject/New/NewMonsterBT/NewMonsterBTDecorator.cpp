// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMonsterBTDecorator.h"
#include "../NewMonsterComponents/NewMonsterStruct.h"
#include "../NewMonster/MyNewBaseMonster.h"
#include "../NewMonster/NewMonsterSpawner.h"
#include "../NewMonster/NewMonsterController.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTDecorator_CheckAttackDistance::UBTDecorator_CheckAttackDistance() {
	NodeName = TEXT("CheckAttackDistance");

}
bool UBTDecorator_CheckAttackDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());

	if (Distance >= Min && Distance <Max) {
		return true;
	}
	else {
		return false;
	}
}


UBTDecorator_ChcckRandomChance::UBTDecorator_ChcckRandomChance() {
	NodeName = TEXT("CheckRanmdomChance");

}
bool UBTDecorator_ChcckRandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);


	uint8 RandomChance = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

	if (RandomChance >= Min && RandomChance < Max) {
		return true;
	}
	else {
		return false;
	}
}

UBTDecorator_CheckMonsterBroken::UBTDecorator_CheckMonsterBroken() {
	NodeName = TEXT("CheckBroken");
}
bool UBTDecorator_CheckMonsterBroken::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AMyNewBaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return false;
	}

	int32 BrokenState = ControllingPawn->GetBrokenState();
	int32 Flag = BrokenState & CheckBrokenType;

	if (Flag != 0) {
		return false;
	}
	else {
		return true;
	}
}