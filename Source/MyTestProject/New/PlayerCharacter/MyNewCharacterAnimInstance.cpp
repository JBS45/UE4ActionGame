// Fill out your copyright notice in the Description page of Project Settings.

#include "MyNewCharacterAnimInstance.h"
#include "../../MyTestProject.h"
#include "MyNewCharacterAnimInstance.h"
#include "MyNewCharacter.h"
#include "MyNewPlayerController.h"


UMyNewCharacterAnimInstance::UMyNewCharacterAnimInstance() {

}
void UMyNewCharacterAnimInstance::NativeBeginPlay() {
	auto Pawn = TryGetPawnOwner();
	auto Character = Cast<AMyNewCharacter>(Pawn);
	PlayerController = Character->GetPlayerController();
	PlayerController->ChangePlayerState.AddUObject(this, &UMyNewCharacterAnimInstance::ChangePlayerState);
	PlayerController->ChangeActionState.AddUObject(this, &UMyNewCharacterAnimInstance::ChangeActionState);
	PlayerController->ChangeWeaponType.AddUObject(this, &UMyNewCharacterAnimInstance::ChangeWeapon);
}
void UMyNewCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn)) {
		CharSpeed = MakeCharSpeed(Pawn);
		auto Character = Cast<AMyNewCharacter>(Pawn);
	}
	CheckMontagePlaying();
}
void UMyNewCharacterAnimInstance::PlayAnimMontage(UAnimMontage* montage) {
	if (montage != nullptr) {
		IsPlayingMontageAnim = true;	
		StopAllMontages(0.1f);
		Montage_Play(montage, 1.0f);
	}
}
float UMyNewCharacterAnimInstance::MakeCharSpeed(APawn* pawn) {
	float Dist = pawn->GetVelocity().Size();
	FVector VelocityXY = FVector(pawn->GetVelocity().X, pawn->GetVelocity().Y, 0.0f);
	VelocityXY.Normalize();
	VelocityXY *= Dist;
	return VelocityXY.Size();
}

void UMyNewCharacterAnimInstance::CheckMontagePlaying() {
	if (IsPlayingMontageAnim) {
		if (!IsAnyMontagePlaying())
		{
			IsPlayingMontageAnim = false;
			PlayerController->ChangeActionState.Broadcast(ENewActionState::E_NONE);
			InputBuffer->NotifyReset();
		}
	}
}
void UMyNewCharacterAnimInstance::ChangeWeapon(ENewWeaponType type) {
	WeaponType = type;
}
void UMyNewCharacterAnimInstance::ChangePlayerState(ENewPlayerState state) {
	PlayerState = state;
}
void UMyNewCharacterAnimInstance::ChangeActionState(ENewActionState state) {
	ActionState = state;
}
void UMyNewCharacterAnimInstance::SetInputBuffer(IResetInputBuffer* input) {
	InputBuffer = input;
}