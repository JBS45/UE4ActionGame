// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMonsterBTService.h"
#include "../NewMonsterComponents/NewMonsterStruct.h"
#include "../NewMonster/MyNewBaseMonster.h"
#include "../NewMonster/NewMonsterSpawner.h"
#include "../NewMonster/NewMonsterController.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckDistance::UBTService_CheckDistance() {
	NodeName = TEXT("CheckDistance");
	Interval = 0.5f;
}

void UBTService_CheckDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto Controller = Cast<ANewMonsterController>(OwnerComp.GetAIOwner());
	auto ControllingPawn = Controller->GetCurrnetMonster();

	if (ControllingPawn == nullptr) return;

	auto Target = Cast<AMyNewCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANewMonsterController::TargetPlayerKey));
	if (IsValid(Target)) {
		float Distance = FVector::Dist(Target->GetActorLocation(), ControllingPawn->GetActorLocation());
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(ANewMonsterController::TargetDistanceKey, Distance);
	}
	else {
		float Distance = MAX_FLT;
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(ANewMonsterController::TargetDistanceKey, Distance);
	}
}

UBTService_MakeRandom::UBTService_MakeRandom() {
	NodeName = TEXT("MakeRandom");
	Interval = 1.0f;
}

void UBTService_MakeRandom::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	int Random = FMath::RandRange(0, 9);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(ANewMonsterController::RandomKey, Random);
}

UBTService_CheckTarget::UBTService_CheckTarget() {
	NodeName = TEXT("CheckInArea");
	Interval = 1.5f;
}

void UBTService_CheckTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto Controller = Cast<ANewMonsterController>(OwnerComp.GetAIOwner());

	if (Controller->GetCurrentConditionState() != EMonsterConditionState::E_IDLE) {
		return;
	}
	if (!IsValid(Controller)) {
		return;
	}

	auto Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANewMonsterController::TargetPlayerKey);
	AMyNewCharacter* Player = Cast<AMyNewCharacter>(Target);

	if (!IsValid(Player)) {
		return;
	}

	if ((Player->IsAlive() == false)
		||(Controller->GetCurrnetMonster()->GetMonsterArea()->GetIsPlayerInArea()==false)) {
		Target = nullptr;
		Controller->ChangeStateDel.Broadcast(ENewMonsterState::E_IDLE);
	}
}