// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimNotify.h"
#include "BaseMonster.h"
#include "Boss.h"
#include "MonsterAIController.h"
#include "../BaseEnum.h"
#include "../Player/BaseCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "../Components/MyInterface.h"
#include "MonsterAnimInstance.h"



FString UMonsterRoar::GetNotifyName_Implementation() const {
	return L"Roar";
}

void UMonsterRoar::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Monster->Roar();
	}
}

FString UEndPlayAnim::GetNotifyName_Implementation() const {
	return L"EndAnim";
}
void UEndPlayAnim::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
	if (Monster == nullptr) {
		return;
	}
	auto MonsterController = Cast<AMonsterAIController>(Monster);
	if (IsValid(MonsterController)) {
		MonsterController->GetBlackboardComponent()->SetValueAsBool(AMonsterAIController::IsAnimPlaying, false);
		//MonsterController
	}
	auto Rotation = Monster->GetActorRotation();
	FRotator InitRotation = FRotator(0, Rotation.Yaw, 0);
	Monster->SetActorRotation(InitRotation);
}

FString UMonsterJumpAttack::GetNotifyName_Implementation() const {
	return L"JumpingMonster";
}

void UMonsterJumpAttack::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<ABaseMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		if (!Monster->GetCharacterMovement()->IsFalling()) {
			Monster->GetAnimInst()->Montage_SetNextSection(CurrentSection, NextSection);
		}
	}
}

FString UMonsterLaunchToPlayer::GetNotifyName_Implementation() const {
	return L"JumpStart";
}

void UMonsterLaunchToPlayer::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<ABaseMonster>(MeshComp->GetOwner());

	if (IsValid(Monster)) {
		auto Target = Cast<ABaseCharacter>(Monster->GetMonsterController()->GetTarget());
		if (IsValid(Target)) {
			FVector Direction = Target->GetActorLocation() - Monster->GetActorLocation();
			float Dist = FVector::Distance(Target->GetActorLocation(), Monster->GetActorLocation());
			Direction.Normalize();
			Monster->LaunchCharacter(FVector(Dist*Direction.X, Dist*Direction.Y, Height), false, false);
		}
	}
}

FString UMonsterRadialDamage::GetNotifyName_Implementation() const {
	return L"RadialDamage";
}

void UMonsterRadialDamage::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<ABaseMonster>(MeshComp->GetOwner());


	if (IsValid(Monster)) {
		TArray<FHitResult> Results;
		TArray<AActor*> Ignore;

		Ignore.Add(Monster);
		UKismetSystemLibrary::SphereTraceMultiByProfile(Monster->GetWorld(), Monster->GetActorLocation(), Monster->GetActorLocation(), Range,
			TEXT("MonsterDamage"), true, Ignore, EDrawDebugTrace::ForOneFrame, Results, true, FLinearColor::Red, FLinearColor::Blue, 1.0f);

		if (Results.Num() <= 0) return;

		bool IsWeak = false;
		int32 OutFinalDamage = 0;

		TArray<AActor*> repeatedActor;

		for (auto Hit : Results) {

			auto Character = Cast<IDamageInterface>(Hit.Actor);

			if (Character != nullptr && !repeatedActor.Contains(Hit.GetActor())) {
				Character->TakeDamageFunc(IsWeak, OutFinalDamage, Monster, Hit, DamageRate*Monster->GetDamage(), DamageType, KnockBack);
				repeatedActor.Add(Hit.GetActor());
			}
		}
	}
}