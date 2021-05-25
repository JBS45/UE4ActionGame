// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterAnimNotifyCollection.h"
#include "../NewMonster/MyNewBaseMonster.h"
#include "../NewMonster/NewMonsterController.h"
#include "../NewMonster/NewMonsterAnimInstance.h"
#include "../PlayerCharacter/MyNewCharacter.h"
#include "../NewMonster/MyNewBossMonster.h"
#include "../NewMonsterComponents/NewMonsterProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
 
FString UConditionExit::GetNotifyName_Implementation() const {
	return L"ConditionExit";
}
void UConditionExit::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto Monster = Cast<AMyNewBaseMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Monster->GetMonsterController()->ChangeConditionStateDel.Broadcast(EMonsterConditionState::E_IDLE);
	}
}

FString UEnableAttack::GetNotifyName_Implementation() const {
	return L"NewMonsterAttack";
}

void UEnableAttack::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) {

}
void UEnableAttack::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) {
	auto Monster = Cast<AMyNewBaseMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Monster->AttackCheck(AttackPart, DamageRate, DamageType,KnockBackDistance);
	}
}
void UEnableAttack::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	auto Monster = Cast<AMyNewBaseMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Monster->ClearDamagedCharacter();
	}
}

FString UMonsterJumping::GetNotifyName_Implementation() const {
	return L"JumpingMonster";
}

void UMonsterJumping::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<AMyNewBaseMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		if (!Monster->GetCharacterMovement()->IsFalling()) {
			Monster->GetAnimInst().Montage_SetNextSection(CurrentSection, NextSection);
		}
	}
}

FString UMonsterJumpToPlayer::GetNotifyName_Implementation() const {
	return L"JumpStart";
}

void UMonsterJumpToPlayer::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<AMyNewBaseMonster>(MeshComp->GetOwner());

	if (IsValid(Monster)) {
		auto Target = Cast<AMyNewCharacter>(Monster->GetMonsterController()->GetTarget());
		if (IsValid(Target)) {
			FVector Direction = Target->GetActorLocation() - Monster->GetActorLocation();
			float Dist = FVector::Distance(Target->GetActorLocation(), Monster->GetActorLocation())-100.0f;
			Direction.Normalize();
			Monster->LaunchCharacter(FVector(Dist*Direction.X, Dist*Direction.Y, Height), false, false);
		}
	}
}

FString URadialDamage::GetNotifyName_Implementation() const {
	return L"RadialDamage";
}

void URadialDamage::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) {
	Super::Notify(MeshComp, Animation);

	auto Monster = Cast<AMyNewBaseMonster>(MeshComp->GetOwner());

	

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

			auto Character = Cast<INewDamageInterface>(Hit.Actor);

			if (Character != nullptr && !repeatedActor.Contains(Hit.GetActor())) {
				Character->TakeAttack(Hit, Monster->GetDamage()*DamageRate,NULL, nullptr, IsWeak, DamageType, KnockBack);
				repeatedActor.Add(Hit.GetActor());
			}
		}
	}
}
FString UThrowStone::GetNotifyName_Implementation() const
{
	return L"ThrowStone";
}

void UThrowStone::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	Monster = Cast<AMyNewBossMonster>(MeshComp->GetOwner());
	if (IsValid(Monster)) {
		Rock = Monster->SpawnProjectile(DamageRange, DamageRate, KnockBackDistance);
	}


}
void UThrowStone::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);

	if (IsValid(Monster)) {
		FVector Location = (MeshComp->GetSocketLocation("Muzzle_01") + MeshComp->GetSocketLocation("Muzzle_02")) / 2;
		Rock->SetActorLocation(Location);
	}

}
void UThrowStone::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (IsValid(Monster)) {
		Monster->ShootProjectile(Rock);
	}
}