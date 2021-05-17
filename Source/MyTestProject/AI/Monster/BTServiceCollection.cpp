// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceCollection.h"
#include "../../BaseEnum.h"
#include "../../Monster/MonsterAIController.h"
#include "../../Monster/BaseMonster.h"
#include "../../Monster/MonsterArea.h"
#include "../../Player/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"


UBTService_FindTargetForMonster::UBTService_FindTargetForMonster() {
	NodeName = TEXT("FindTarget");
	Interval = 1.0f;
}


void UBTService_FindTargetForMonster::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());
	auto Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (ControllingPawn == nullptr) return;

	auto target = ControllingPawn->GetTarget();
	if (target == nullptr) {
		Controller->SetTarget(nullptr);
		return;
	}

	float dist = FVector::DistXY(target->GetActorLocation(), ControllingPawn->GetActiveArea()->GetActorLocation());
	if (dist <= ControllingPawn->GetDetectRange() && ControllingPawn->GetActiveArea()->GetIsPlayerInRange()) {
		Controller->SetTarget(target);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetPlayerKey, target);
	}
	else {
		Controller->SetTarget(nullptr);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetPlayerKey, nullptr);
	}
}

UBTService_SearchForMonster::UBTService_SearchForMonster() {
	NodeName = TEXT("Search");
	Interval = 1.0f;
}

void UBTService_SearchForMonster::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());

	auto Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (ControllingPawn == nullptr) return;

	auto target = ControllingPawn->GetTarget();
	if (target == nullptr) {
		Controller->SetTarget(nullptr);
		return;
	}

	TArray<FHitResult> DetectResult;
	TArray<AActor*> Ignore;
	Ignore.Add(ControllingPawn);

	UKismetSystemLibrary::SphereTraceMultiByProfile(ControllingPawn->GetWorld(),
		ControllingPawn->GetActorLocation(),
		ControllingPawn->GetActorLocation(),
		ControllingPawn->GetDetectRange(),
		TEXT("MonsterDamage"),
		true, Ignore, EDrawDebugTrace::ForOneFrame,
		DetectResult, true, FLinearColor::Blue, FLinearColor::Yellow, 1.0f);

	TArray<AActor*> repeatedActor;

	for (auto DetectCharacter : DetectResult) {

		auto Character = Cast<IDamageInterface>(DetectCharacter.Actor);

		if (Character != nullptr && !repeatedActor.Contains(DetectCharacter.GetActor())) {
			Controller->SetTarget(Cast<ABaseCharacter>(DetectCharacter.Actor));
			repeatedActor.Add(DetectCharacter.GetActor());
		}
	}
}

UBTService_CheckCanChangeBattle::UBTService_CheckCanChangeBattle() {
	NodeName = TEXT("CheckChangeBattle");
	Interval = 1.0f;
}

void UBTService_CheckCanChangeBattle::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());
	auto Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	TESTLOG(Warning, TEXT("awwds"));
	if (ControllingPawn == nullptr) return;

	auto target = ControllingPawn->GetTarget();

	if (Controller->GetIsFoundPlayer()) {
		Timer += DeltaSeconds;
		if (Timer >= 10.0f) {
			Controller->StateChangeDel.Broadcast(EMonsterState::E_BATTLE);
		}
	}
	else {
		Timer = 0;
	}
}


UBTService_Random::UBTService_Random() {
	NodeName = TEXT("Random");
	Interval = 1.5f;
}

void UBTService_Random::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn == nullptr) return;

	int Random = FMath::RandRange(0, 9);
	OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMonsterAIController::RandIntKey, Random);
}

UBTService_CheckTargetInArea::UBTService_CheckTargetInArea() {
	NodeName = TEXT("CheckInArea");
	Interval = 1.5f;
}

void UBTService_CheckTargetInArea::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn == nullptr) return;

	if (!(ControllingPawn->GetActiveArea()->GetIsPlayerInRange())) {
		ControllingPawn->GetMonsterController()->ChangeMonsterState(EMonsterState::E_IDLE);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetPlayerKey, nullptr);
	}

}

UBTService_CheckTargetDistance::UBTService_CheckTargetDistance() {
	NodeName = TEXT("CheckPlayerDistance");
	Interval = 0.5f;
	MaxChaseDistance = 3500.0f;
}


void UBTService_CheckTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());

	if (ControllingPawn == nullptr) return;

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	float Distance = FVector::Dist(Target->GetActorLocation(), ControllingPawn->GetActorLocation());
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMonsterAIController::TargetDistanceKey, Distance);

	if (Distance > MaxChaseDistance) {
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetPlayerKey, nullptr);
		Cast<AMonsterAIController>(OwnerComp.GetAIOwner())->ChangeMonsterState(EMonsterState::E_IDLE);
	}
}

UBTService_DecideForward::UBTService_DecideForward() {
	NodeName = TEXT("DecideForward");
	Interval = 0.5f;
}
void UBTService_DecideForward::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	bool Result = true;

	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());
	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));

	FVector TargetDirection = (Target->GetActorLocation() - ControllingPawn->GetActorLocation());
	FVector TargetDirection2D = FVector(TargetDirection.X, TargetDirection.Y, 0);
	TargetDirection2D.Normalize();
	float Dot = FVector::DotProduct(Target->GetActorForwardVector(), TargetDirection2D);

	float back = FMath::Cos(FMath::DegreesToRadians(30));
	TESTLOG(Warning, TEXT("%f"), back);
	if (Dot <= back) {
		Result = false;
	}


	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMonsterAIController::IsForwardDirection, Result);
}


UBTService_CheckTargetAlive::UBTService_CheckTargetAlive() {
	NodeName = TEXT("CheckAlive");
	Interval = 0.5f;
}

void UBTService_CheckTargetAlive::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	bool Result = true;
	auto ControllingPawn = Cast<ABaseMonster>((OwnerComp.GetAIOwner())->GetPawn());
	auto Controller = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());

	auto Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));

	if (Target->GetState() == ECharacterState::E_DEAD) {
		Controller->StateChangeDel.Broadcast(EMonsterState::E_IDLE);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMonsterAIController::TargetPlayerKey, nullptr);
	}



	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMonsterAIController::IsForwardDirection, Result);
}
