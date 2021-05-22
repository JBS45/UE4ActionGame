// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskCollection.h"
#include "../../Monster/MonsterAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../Monster/BaseMonster.h"
#include "../../Monster/MonsterArea.h"
#include "../../Monster/MonsterAnimInstance.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "../../Monster/MonsterArea.h"
#include "../../Player/BaseCharacter.h"
#include "Tasks/AITask_MoveTo.h"


UBTTask_MonsterPatrol::UBTTask_MonsterPatrol() {
	NodeName = TEXT("Patrol");
}

EBTNodeResult::Type UBTTask_MonsterPatrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto PatrolPoint = ControllingPawn->GetActiveArea()->GetPatrolPoint();
	int32 Length = PatrolPoint.Num();
	if (Length == 0) {
		return EBTNodeResult::Failed;
	}

	int32 Index = OwnerComp.GetBlackboardComponent()->GetValueAsInt("NextPatrolIndex");
	if (Index >= Length) {
		Index = 0;
	}


	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) {
		return EBTNodeResult::Failed;
	}
	FNavLocation NextLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(PatrolPoint[Index]->GetActorLocation(), 100.0f, NextLocation)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAIController::TargetLocationKey, NextLocation.Location);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt("NextPatrolIndex", Index + 1);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

UBTTask_FindClosePatrolPoint::UBTTask_FindClosePatrolPoint() {
	NodeName = TEXT("FindColsePoint");
}

EBTNodeResult::Type UBTTask_FindClosePatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto PatrolPoint = ControllingPawn->GetActiveArea()->GetPatrolPoint();
	int32 Length = PatrolPoint.Num();
	if (Length == 0) {
		return EBTNodeResult::Failed;
	}

	int32 MinIndex = 0;
	float MinDist = MAX_FLT;
	for (int i = 0; i < Length; ++i) {
		float dist = FVector::Dist(ControllingPawn->GetActorLocation(), PatrolPoint[i]->GetActorLocation());
		if (dist < MinDist) {
			MinDist = dist;
			MinIndex = i;
		}
	}

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
	if (NavSystem == nullptr) {
		return EBTNodeResult::Failed;
	}
	FNavLocation NextLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(PatrolPoint[MinIndex]->GetActorLocation(), 100.0f, NextLocation)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMonsterAIController::TargetLocationKey, NextLocation.Location);
		OwnerComp.GetBlackboardComponent()->SetValueAsInt("NextPatrolIndex", MinIndex + 1);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;


}


UBTTask_PlayAnim::UBTTask_PlayAnim() {
	NodeName = TEXT("PlayAnim");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}
	ControllingPawn->GetAnimInst()->PlayAnimation(Anim, 1.0f);
	IsBegin = true;

	return EBTNodeResult::InProgress;
}
void UBTTask_PlayAnim::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	if (!IsBegin) return;

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!ControllingPawn->GetAnimInst()->Montage_IsPlaying(Anim)) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

UBTTask_PlayAnimToForward::UBTTask_PlayAnimToForward() {
	NodeName = TEXT("PlayAnimForward");
	distance = 200.0f;
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayAnimToForward::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	FVector Forward = ControllingPawn->GetActorForwardVector();

	FVector Dest = ControllingPawn->GetActorLocation() + (Forward * distance);

	ControllingPawn->GetMonsterController()->MoveToLocation(Dest, 10.0f, true, false);
	//ControllingPawn->LaunchCharacter((Forward * distance),true, false);
	ControllingPawn->GetAnimInst()->PlayAnimation(Anim, 1.0f);



	IsBegin = true;

	return EBTNodeResult::InProgress;
}
void UBTTask_PlayAnimToForward::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	if (!IsBegin) return;

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!ControllingPawn->GetAnimInst()->Montage_IsPlaying(Anim)) {
		ControllingPawn->GetCharacterMovement()->StopActiveMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

UBTTask_PlayTwoAnim::UBTTask_PlayTwoAnim() {
	NodeName = TEXT("PlayRotateAnim");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_PlayTwoAnim::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	ControllingPawn->GetAnimInst()->PlayAnimation(Anim, 1.0f);

	//right
	if (Degree >= 0) {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(1.0f);
	}
	//Left
	else {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(-1.0f);
	}

	float time = Anim->GetPlayLength();
	Lerpspeed = Degree / time;
	DestRotation = ControllingPawn->GetActorRotation() + FRotator(0, Degree, 0);



	return EBTNodeResult::InProgress;
}
void UBTTask_PlayTwoAnim::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	if (Target == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	FRotator Temp = ControllingPawn->GetActorRotation();
	ControllingPawn->SetActorRelativeRotation(FRotator(Temp.Pitch, Temp.Yaw + (Lerpspeed*DeltaSeconds), Temp.Roll));

	if (!ControllingPawn->GetAnimInst()->Montage_IsPlaying(Anim)) {
		ControllingPawn->GetCharacterMovement()->StopActiveMovement();
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(0.0f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

UBTTask_TurnForMonster::UBTTask_TurnForMonster() {
	NodeName = TEXT("Turn");
	bNotifyTick = true;
	LerpSpped = 3.0f;
}

EBTNodeResult::Type UBTTask_TurnForMonster::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	if (Target == nullptr) {
		return EBTNodeResult::Failed;
	}

	TargetVector = FVector((Target->GetActorLocation() - ControllingPawn->GetActorLocation()).X, (Target->GetActorLocation() - ControllingPawn->GetActorLocation()).Y, 0);
	TargetVector.Normalize();

	Dest = FindRotation(ControllingPawn);

	return EBTNodeResult::InProgress;
}

void UBTTask_TurnForMonster::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	auto Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));
	if (Target == nullptr) {
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	ControllingPawn->SetActorRelativeRotation(UKismetMathLibrary::RInterpTo(ControllingPawn->GetActorRotation(), Dest, DeltaSeconds, LerpSpped));


	auto angle = UKismetMathLibrary::Acos(UKismetMathLibrary::Dot_VectorVector(ControllingPawn->GetActorForwardVector(), TargetVector));
	angle = FMath::RadiansToDegrees(angle);

	if (angle <= 2 * LerpSpped) {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(0.0f);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else {

		Dest = FindRotation(ControllingPawn);
	}

}
FRotator UBTTask_TurnForMonster::FindRotation(ABaseMonster* ControllingPawn) {

	FRotator Result;
	float dotRight = FVector::DotProduct(ControllingPawn->GetActorRightVector(), TargetVector);

	//right
	if (dotRight >= 0) {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(1.0f);
	}
	//Left
	else {
		Cast<UMonsterAnimInstance>(ControllingPawn->GetAnimInst())->SetRightRotate(-1.0f);
	}


	Result = UKismetMathLibrary::FindLookAtRotation(ControllingPawn->GetActorForwardVector(), TargetVector);
	return Result;
}


UBTTaskNode_ChaseTarget::UBTTaskNode_ChaseTarget() {
	NodeName = TEXT("ChaseTarget");
}

EBTNodeResult::Type UBTTaskNode_ChaseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}


	Target = Cast<ABaseCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMonsterAIController::TargetPlayerKey));

	auto ChaseResult = ControllingPawn->GetMonsterController()->MoveToActor(Target, AcceptableRadius);

	if (ChaseResult != EPathFollowingRequestResult::Failed) {
		TESTLOG(Warning, TEXT("Success"));
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}

UBTTaskNode_ChaseFail::UBTTaskNode_ChaseFail() {
	NodeName = TEXT("ChaseFail");
}

EBTNodeResult::Type UBTTaskNode_ChaseFail::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto Control = Cast<AMonsterAIController>(OwnerComp.GetAIOwner());
	if (Control == nullptr) {
		return EBTNodeResult::Failed;
	}

	Control->ChangeMonsterState(EMonsterState::E_IDLE);


	return EBTNodeResult::Succeeded;
}
UBTTask_Roar::UBTTask_Roar() {
	NodeName = TEXT("Roar");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Roar::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (ControllingPawn == nullptr) {
		return EBTNodeResult::Failed;
	}
	ControllingPawn->GetAnimInst()->PlayAnimation(Anim, 1.0f);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMonsterAIController::CanRoar, true);

	return EBTNodeResult::InProgress;
}
void UBTTask_Roar::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {

	auto ControllingPawn = Cast<ABaseMonster>(OwnerComp.GetAIOwner()->GetPawn());

	if (!ControllingPawn->GetAnimInst()->Montage_IsPlaying(Anim)) {
		ControllingPawn->GetCharacterMovement()->StopActiveMovement();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}