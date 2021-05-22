// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMonsterBTTask.h"
#include "../NewMonsterComponents/NewMonsterStruct.h"
#include "../NewMonster/MyNewBaseMonster.h"
#include "../NewMonster/NewMonsterSpawner.h"
#include "../NewMonster/NewMonsterController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tasks/AITask_MoveTo.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "../NewMonster/NewMonsterAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"


UBTTask_RandomPointInArea::UBTTask_RandomPointInArea() {
	NodeName = TEXT("PatrolPointInArea");
	AcceptableRadius = 10.0f;
}

EBTNodeResult::Type UBTTask_RandomPointInArea::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Controller = Cast<ANewMonsterController>(OwnerComp.GetAIOwner());
	auto ControllingPawn = Cast<AMyNewBaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto PatrolPoint = ControllingPawn->GetMonsterArea()->FindPatrolPoint();
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(ANewMonsterController::TargetLocationKey, PatrolPoint);

	return EBTNodeResult::Succeeded;
}

UBTTask_ChangeToIDLE::UBTTask_ChangeToIDLE() {
	NodeName = TEXT("IdleState");
}

EBTNodeResult::Type UBTTask_ChangeToIDLE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Controller = Cast<ANewMonsterController>(OwnerComp.GetAIOwner());
	if (Controller == nullptr) {
		return EBTNodeResult::Failed;
	}

	Controller->ChangeStateDel.Broadcast(ENewMonsterState::E_IDLE);

	return EBTNodeResult::Succeeded;

}

UBTTask_ChangeToBattle::UBTTask_ChangeToBattle() {
	NodeName = TEXT("BattleState");
}

EBTNodeResult::Type UBTTask_ChangeToBattle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Controller = Cast<ANewMonsterController>(OwnerComp.GetAIOwner());
	if (Controller == nullptr) {
		return EBTNodeResult::Failed;
	}

	Controller->ChangeStateDel.Broadcast(ENewMonsterState::E_BATTLE);

	return EBTNodeResult::Succeeded;

}

UBTTask_MonsterTurning::UBTTask_MonsterTurning() {
	NodeName = TEXT("Turning");
	bNotifyTick = true;
	LerpSpeed = 3.0f;
}

EBTNodeResult::Type UBTTask_MonsterTurning::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AMyNewBaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<AMyNewCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANewMonsterController::TargetPlayerKey));

	if (Target == nullptr) {
		return EBTNodeResult::Failed;
	}

	TargetVector = FVector((Target->GetActorLocation() - ControllingPawn->GetActorLocation()).X, (Target->GetActorLocation() - ControllingPawn->GetActorLocation()).Y, 0);
	TargetVector.Normalize();

	Dest = FindRotation(*ControllingPawn);

	return EBTNodeResult::InProgress;
}

void UBTTask_MonsterTurning::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	auto ControllingPawn = Cast<AMyNewBaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANewMonsterController::TargetPlayerKey));
	if (Target == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	ControllingPawn->SetActorRelativeRotation(UKismetMathLibrary::RInterpTo(ControllingPawn->GetActorRotation(), Dest, DeltaSeconds, LerpSpeed));


	auto angle = UKismetMathLibrary::Acos(UKismetMathLibrary::Dot_VectorVector(ControllingPawn->GetActorForwardVector(), TargetVector));
	angle = FMath::RadiansToDegrees(angle);

	if (angle <= 2 * LerpSpeed) {
		ControllingPawn->GetAnimInst().SetTurnSpeed(0.0f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else {

		Dest = FindRotation(*ControllingPawn);
	}

}
FRotator UBTTask_MonsterTurning::FindRotation(AMyNewBaseMonster& monster) {

	FRotator Result;
	float dotRight = FVector::DotProduct(monster.GetActorRightVector(), TargetVector);

	//right
	if (dotRight >= 0) {
		monster.GetAnimInst().SetTurnSpeed(1.0f);
	}
	//Left
	else {
		monster.GetAnimInst().SetTurnSpeed(-1.0f);
	}

	Result = UKismetMathLibrary::FindLookAtRotation(monster.GetActorForwardVector(), TargetVector);
	return Result;
}

UBTTask_PlayAnimMontage::UBTTask_PlayAnimMontage() {
	NodeName = TEXT("PlayAnimMontage");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayAnimMontage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AMyNewBaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	auto Controller = Cast<ANewMonsterController>(OwnerComp.GetAIOwner());

	if (!IsValid(ControllingPawn) || !IsValid(Controller)) {
		return EBTNodeResult::Failed;
	}
	ControllingPawn->GetAnimInst().PlayAnimation(Anim, 1.0f);
	Controller->ChangeConditionStateDel.Broadcast(EMonsterConditionState::E_ACTION);
	IsBegin = true;

	return EBTNodeResult::InProgress;
}
void UBTTask_PlayAnimMontage::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	if (!IsBegin) return;

	auto ControllingPawn = Cast<AMyNewBaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	auto Controller = Cast<ANewMonsterController>(OwnerComp.GetAIOwner());

	if (!ControllingPawn->GetAnimInst().Montage_IsPlaying(Anim)) {
		if (Controller->GetCurrentConditionState() <= EMonsterConditionState::E_ACTION) {
			Controller->ChangeConditionStateDel.Broadcast(EMonsterConditionState::E_IDLE);
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

UBTTask_PlayAnimMontageForward::UBTTask_PlayAnimMontageForward() {
	NodeName = TEXT("PlayAnimMontageCharge");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayAnimMontageForward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<AMyNewBaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	auto Controller = Cast<ANewMonsterController>(OwnerComp.GetAIOwner());

	if (!IsValid(ControllingPawn) || !IsValid(Controller)) {
		return EBTNodeResult::Failed;
	}
	ControllingPawn->GetAnimInst().PlayAnimation(Anim, 1.0f);

	ForwardDirection = ControllingPawn->GetActorForwardVector();

	Controller->ChangeConditionStateDel.Broadcast(EMonsterConditionState::E_ACTION);

	IsBegin = true;

	return EBTNodeResult::InProgress;
}
void UBTTask_PlayAnimMontageForward::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	if (!IsBegin) return;

	auto ControllingPawn = Cast<AMyNewBaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	auto Controller = Cast<ANewMonsterController>(OwnerComp.GetAIOwner());

	ControllingPawn->AddMovementInput(ForwardDirection, LerpSpeed);

	if (!ControllingPawn->GetAnimInst().Montage_IsPlaying(Anim)) {
		if (Controller->GetCurrentConditionState() <= EMonsterConditionState::E_ACTION) {
			Controller->ChangeConditionStateDel.Broadcast(EMonsterConditionState::E_IDLE);
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

UBTTaskNode_ChasePlayer::UBTTaskNode_ChasePlayer() {
	NodeName = TEXT("ChasePlayer");
}

EBTNodeResult::Type UBTTaskNode_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Controller= Cast<ANewMonsterController>(OwnerComp.GetAIOwner());
	if (Controller == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<AMyNewCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ANewMonsterController::TargetPlayerKey));

	if (!IsValid(Target)) {
		return EBTNodeResult::Failed;
	}
	auto ChaseResult = Controller->MoveToActor(Target, AcceptableRadius);

	if (ChaseResult != EPathFollowingRequestResult::Failed) {
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}