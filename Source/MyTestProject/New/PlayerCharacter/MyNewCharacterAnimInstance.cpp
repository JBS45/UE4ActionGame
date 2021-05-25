// Fill out your copyright notice in the Description page of Project Settings.

#include "MyNewCharacterAnimInstance.h"
#include "../../MyTestProject.h"
#include "MyNewCharacterAnimInstance.h"
#include "MyNewCharacter.h"
#include "MyNewPlayerController.h"
#include "Kismet/KismetMathLibrary.h"


UMyNewCharacterAnimInstance::UMyNewCharacterAnimInstance() {
	static ConstructorHelpers::FObjectFinder<UAnimMontage> LeftHitAnim(TEXT("AnimMontage'/Game/New/Character/Animation/Common/LeftHit.LeftHit'"));
	if (LeftHitAnim.Succeeded())
	{
		LeftHit = LeftHitAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RightHitAnim(TEXT("AnimMontage'/Game/New/Character/Animation/Common/RightHit.RightHit'"));
	if (RightHitAnim.Succeeded())
	{
		RightHit = RightHitAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RoarHitAnim(TEXT("AnimMontage'/Game/New/Character/Animation/Common/HitRoar_Common.HitRoar_Common'"));
	if (RoarHitAnim.Succeeded())
	{
		RoarHit = RoarHitAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> KnockBackAnim(TEXT("AnimMontage'/Game/New/Character/Animation/Common/KnockBack_Common.KnockBack_Common'"));
	if (KnockBackAnim.Succeeded())
	{
		KnockBack = KnockBackAnim.Object;
	}
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
void UMyNewCharacterAnimInstance::Init() {
	auto Pawn = TryGetPawnOwner();
	auto Character = Cast<AMyNewCharacter>(Pawn);
	PlayerController = Character->GetPlayerController();
	PlayerController->ChangePlayerState.AddUObject(this, &UMyNewCharacterAnimInstance::ChangePlayerState);
	PlayerController->ChangeActionState.AddUObject(this, &UMyNewCharacterAnimInstance::ChangeActionState);
	PlayerController->ChangeWeaponType.AddUObject(this, &UMyNewCharacterAnimInstance::ChangeWeapon);
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
			switch (PlayerController->GetCurrentActionState()) {
				case ENewActionState::E_ACTION:
				case ENewActionState::E_AFTERACTION:
				case ENewActionState::E_EVADE:
				case ENewActionState::E_HIT:
				case ENewActionState::E_NONE:
					PlayerController->ChangeActionState.Broadcast(ENewActionState::E_NONE);
					break;
			}
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

void UMyNewCharacterAnimInstance::HitEvent(FVector HitDir, FVector right) {
	int Flag = UKismetMathLibrary::Dot_VectorVector(HitDir, right);
	if (Flag >= 0) {
		PlayAnimMontage(RightHit);
	}
	else {
		PlayAnimMontage(LeftHit);
	}
}

void UMyNewCharacterAnimInstance::PlayKnockBack() {
	PlayAnimMontage(KnockBack);
}
void UMyNewCharacterAnimInstance::PlayRoarHit() {
	PlayAnimMontage(RoarHit);
}
