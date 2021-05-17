// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecoratorCollection.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Monster/MonsterAIController.h"
#include "../../Monster/BaseMonster.h"
#include "../../Player/BaseCharacter.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance() {
	NodeName = TEXT("CanAttackDistance");

}
bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return false;
	}
	;
	float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());

	if (Distance <= 0 && Distance <= ControllingPawn->GetAttackRange()) {
		return true;
	}
	else {
		return false;
	}
}


UBTDecorator_TargetInRange::UBTDecorator_TargetInRange() {
	NodeName = TEXT("TargetInRange");
}

bool UBTDecorator_TargetInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return false;
	}

	float Distance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(GetSelectedBlackboardKey());

	if (Distance > Min && Distance <= Max) {
		return true;
	}
	else {
		return false;
	}
}

UBTDecorator_RandomChance::UBTDecorator_RandomChance() {
	NodeName = TEXT("RandomCheck");
}

bool UBTDecorator_RandomChance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return false;
	}

	float Random = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GetSelectedBlackboardKey());

	if (Random > Min&&Random <= Max) {
		return true;
	}
	else {
		return false;
	}
}
UBTDecorator_LeftSide::UBTDecorator_LeftSide() {
	NodeName = TEXT("TargetInLeftSide");
}

bool UBTDecorator_LeftSide::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return false;
	}

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!IsValid(Target)) return false;
	FVector TargetDirection = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	TargetDirection.Normalize();

	float forward = FVector::DotProduct(ControllingPawn->GetActorForwardVector(), TargetDirection);
	float Right = FVector::DotProduct(ControllingPawn->GetActorRightVector(), TargetDirection);

	if (forward < 0.5f&&Right < 0) {
		return true;
	}
	else {
		return false;
	}

}

UBTDecorator_RightSide::UBTDecorator_RightSide() {
	NodeName = TEXT("TargetInRightSide");
}

bool UBTDecorator_RightSide::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return false;
	}

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!IsValid(Target)) return false;
	FVector TargetDirection = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	TargetDirection.Normalize();

	float forward = FVector::DotProduct(ControllingPawn->GetActorForwardVector(), TargetDirection);
	float Right = FVector::DotProduct(ControllingPawn->GetActorRightVector(), TargetDirection);

	if (forward < 0.5f&&Right > 0) {
		return true;
	}
	else {
		return false;
	}


}

UBTDecorator_ForwardSide::UBTDecorator_ForwardSide() {
	NodeName = TEXT("Forward");
}

bool UBTDecorator_ForwardSide::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return false;
	}

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey()));
	if (!IsValid(Target)) return false;
	FVector TargetDirection = Target->GetActorLocation() - ControllingPawn->GetActorLocation();
	TargetDirection.Normalize();

	float forward = FVector::DotProduct(ControllingPawn->GetActorForwardVector(), TargetDirection);

	if (FMath::Abs(forward) >= 0.5f) {
		return true;
	}
	else {
		return false;
	}


}

UBTDecorator_CheckBroken::UBTDecorator_CheckBroken() {
	NodeName = TEXT("CheckBrokenPart");
}
bool UBTDecorator_CheckBroken::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const {
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
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