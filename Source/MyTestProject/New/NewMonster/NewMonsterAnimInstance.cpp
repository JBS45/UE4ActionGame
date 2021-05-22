// Fill out your copyright notice in the Description page of Project Settings.


#include "NewMonsterAnimInstance.h"
#include "MyNewBaseMonster.h"

UNewMonsterAnimInstance::UNewMonsterAnimInstance() {

}
void UNewMonsterAnimInstance::NativeBeginPlay() {
	Super::NativeBeginPlay();
}
void UNewMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {
		float Dist = Pawn->GetVelocity().Size();
		FVector VelocityXY = FVector(Pawn->GetVelocity().X, Pawn->GetVelocity().Y, 0.0f);
		VelocityXY.Normalize();
		VelocityXY *= Dist;
		CharSpeed = VelocityXY.Size();

	}
	if (Owner == nullptr) {
		Owner = Cast<AMyNewBaseMonster>(Pawn);
	}
	if (IsValid(Owner)) {
		CurrentMonsterState = Owner->GetCurrentMonsterState();
		CurrentConditionState = Owner->GetCurrentConditionState();
	}
}

void UNewMonsterAnimInstance::PlayAnimation(UAnimMontage* montage, float rate) {
	if (montage == nullptr) return;

	if (!Montage_IsPlaying(montage)) {
		Montage_Play(montage, rate);
	}
}

void UNewMonsterAnimInstance::ChangeMonsterState(const ENewMonsterState state) {
	if (CurrentMonsterState == state) return;
	CurrentMonsterState = state;
	switch (CurrentMonsterState)
	{
	case ENewMonsterState::E_IDLE:
		break;
	case ENewMonsterState::E_BATTLE:
		break;
	case ENewMonsterState::E_DEAD:
		break;
	}
}
void UNewMonsterAnimInstance::ChangeMonsterConditionState(const EMonsterConditionState state) {
	if (CurrentConditionState == state) return;
	CurrentConditionState = state;
	switch (CurrentConditionState)
	{
	case EMonsterConditionState::E_IDLE:
		break;
	case EMonsterConditionState::E_KNOCKBACK:
		break;
	case EMonsterConditionState::E_STUN:
		break;
	case EMonsterConditionState::E_DOWN:
		break;
	}
}

